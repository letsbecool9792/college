#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
struct sockaddr_in server_address, client_address;
int listen_file_descriptor, connection_file_descriptor;
int read_count, write_count;
int bind_result, listen_result;
int client_address_length;
int match_count, file_read_count;
int mistakes;
unsigned short server_port = 25025;
char server_ip[] = "192.168.24.44";
char receive_buffer[128];
char command_buffer[256];
char list_buffer[1024];
char line_buffer[256];
char file_buffer[1024];
char perm_string[128];
char *file_name_pointer;
FILE *matches_file;
FILE *requested_file;
FILE *perm_file;
void main()
{
    /* Filling server address structure */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_aton(server_ip, &server_address.sin_addr);
    printf("\nTCP FILE SEARCH SERVER\n");
    /* Creating socket */
    listen_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_file_descriptor < 0)
    {
        printf("\nSERVER ERROR: Cannot create socket\n");
        exit(1);
    }
    /* Binding server socket address */
    bind_result = bind(listen_file_descriptor,
                       (struct sockaddr *)&server_address,
                       sizeof(server_address));
    if (bind_result < 0)
    {
        printf("\nSERVER ERROR: Cannot bind\n");
        close(listen_file_descriptor);
        exit(1);
    }
    /* Listening for client connection requests */
    listen_result = listen(listen_file_descriptor, 5);
    if (listen_result < 0)
    {
        printf("\nSERVER ERROR: Cannot listen\n");
        close(listen_file_descriptor);
        exit(1);
    }
    client_address_length = sizeof(client_address);
    /* Keep accepting new clients */
    for (;;)
    {
        printf("\nServer: Waiting for client...\n");
        /* Accept client connection */
        connection_file_descriptor = accept(
            listen_file_descriptor,
            (struct sockaddr *)&client_address,
            &client_address_length
        );
        if (connection_file_descriptor < 0)
        {
            printf("\nCannot accept client connection\n");
            continue;
        }
        printf("\nConnection from client %s accepted\n",
               inet_ntoa(client_address.sin_addr));
        /* SEARCH PHASE: keep taking queries until a match is found */
        for (;;)
        {
            read_count = read(connection_file_descriptor, receive_buffer, 127);
            if (read_count <= 0)
            {
                printf("\nClient disconnected\n");
                break;
            }
            receive_buffer[read_count] = '\0';
            printf("\nClient search query: %s\n", receive_buffer);
            /* Build: find . -maxdepth 1 -name "ab*" -printf "%f\n" > matches.txt */
            strcpy(command_buffer, "find . -maxdepth 1 -name \"");
            strcat(command_buffer, receive_buffer);
            strcat(command_buffer, "*\" -printf \"%f\\n\" > matches.txt 2>/dev/null");
            system(command_buffer);
            /* Read matches.txt into the list buffer */
            list_buffer[0] = '\0';
            match_count = 0;
            matches_file = fopen("matches.txt", "r");
            if (matches_file != NULL)
            {
                while (fgets(line_buffer, sizeof(line_buffer), matches_file) != NULL)
                {
                    strcat(list_buffer, line_buffer);
                    match_count++;
                }
                fclose(matches_file);
            }
            /* No match: tell client to re-enter */
            if (match_count == 0)
            {
                write(connection_file_descriptor, "NO_RESULTS", strlen("NO_RESULTS"));
                printf("No matching files for this query\n");
                continue;
            }
            /* Match found: send the list and move to fetch phase */
            write(connection_file_descriptor, list_buffer, strlen(list_buffer));
            printf("Sent %d matching file name(s)\n", match_count);
            break;
        }
        /* If client left during search, go wait for the next client */
        if (read_count <= 0)
        {
            close(connection_file_descriptor);
            printf("\nClient connection closed.\n");
            continue;
        }
        /* FETCH PHASE: two wrong picks allowed, then send the valid list */
        mistakes = 0;
        for (;;)
        {
            read_count = read(connection_file_descriptor, receive_buffer, 127);
            if (read_count <= 0)
            {
                printf("\nClient disconnected\n");
                break;
            }
            receive_buffer[read_count] = '\0';
            printf("\nClient requested file: %s\n", receive_buffer);
            /* Get the ls -l permission string (-d so a dir shows its own entry) */
            strcpy(command_buffer, "ls -ld \"");
            strcat(command_buffer, receive_buffer);
            strcat(command_buffer, "\" > perm.txt 2>/dev/null");
            system(command_buffer);
            perm_string[0] = '\0';
            perm_file = fopen("perm.txt", "r");
            if (perm_file != NULL)
            {
                fgets(perm_string, sizeof(perm_string), perm_file);
                fclose(perm_file);
            }
            /* Classify from the permission string */
            if (perm_string[0] == '\0')
            {
                mistakes++;
                if (mistakes < 2)
                {
                    write(connection_file_descriptor,
                          "ERROR: no such file, enter again",
                          strlen("ERROR: no such file, enter again"));
                    continue;
                }
            }
            else if (perm_string[0] == 'd')
            {
                mistakes++;
                if (mistakes < 2)
                {
                    write(connection_file_descriptor,
                          "ERROR: this is a directory, cannot read",
                          strlen("ERROR: this is a directory, cannot read"));
                    continue;
                }
            }
            else if (perm_string[3] == 'x' || perm_string[6] == 'x' || perm_string[9] == 'x')
            {
                mistakes++;
                if (mistakes < 2)
                {
                    write(connection_file_descriptor,
                          "ERROR: this is an executable, cannot read",
                          strlen("ERROR: this is an executable, cannot read"));
                    continue;
                }
            }
            else
            {
                /* Valid regular, non-executable file: send it and finish */
                requested_file = fopen(receive_buffer, "r");
                if (requested_file == NULL)
                {
                    write(connection_file_descriptor,
                          "ERROR: cannot open file, enter again",
                          strlen("ERROR: cannot open file, enter again"));
                    continue;
                }
                while ((file_read_count = fread(file_buffer, 1, sizeof(file_buffer), requested_file)) > 0)
                {
                    write(connection_file_descriptor, file_buffer, file_read_count);
                }
                fclose(requested_file);
                printf("File sent to client\n");
                break;
            }
            /* Reached here => mistakes >= 2: send the valid file list */
            system("ls -l > valid.txt 2>/dev/null");
            list_buffer[0] = '\0';
            strcat(list_buffer, "VALID_LIST\n");
            matches_file = fopen("valid.txt", "r");
            if (matches_file != NULL)
            {
                while (fgets(line_buffer, sizeof(line_buffer), matches_file) != NULL)
                {
                    line_buffer[strcspn(line_buffer, "\n")] = '\0';
                    /* Keep regular files with no execute bit anywhere */
                    if (line_buffer[0] == '-' &&
                        line_buffer[3] != 'x' &&
                        line_buffer[6] != 'x' &&
                        line_buffer[9] != 'x')
                    {
                        /* File name is the last word on the ls -l line */
                        file_name_pointer = strrchr(line_buffer, ' ') + 1;
                        strcat(list_buffer, file_name_pointer);
                        strcat(list_buffer, "\n");
                    }
                }
                fclose(matches_file);
            }
            write(connection_file_descriptor, list_buffer, strlen(list_buffer));
            printf("Two wrong picks: sent valid file list\n");
        }
        /* Close current client connection */
        close(connection_file_descriptor);
        printf("\nClient connection closed.\n");
    }
}

/*
OUTPUT
------
(server folder before running: a  ab  abc/  abcd.txt  hello.txt  server  server.c)

~/server$ ./server

TCP FILE SEARCH SERVER

Server: Waiting for client...

Connection from client 192.168.24.44 accepted

Client search query: ab
Sent 3 matching file name(s)

Client requested file: abc

Client requested file: abcd.txt
File sent to client

Client connection closed.

Server: Waiting for client...

Connection from client 192.168.24.44 accepted

Client search query: s
Sent 2 matching file name(s)

Client requested file: server

Client requested file: abc
Two wrong picks: sent valid file list

Client requested file: hello.txt
File sent to client

Client connection closed.

Server: Waiting for client...
^C
*/

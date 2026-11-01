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
unsigned short server_port = 25025;
char server_ip[] = "192.168.24.44";
char receive_buffer[128];
char command_buffer[256];
char list_buffer[1024];
char line_buffer[256];
char file_buffer[1024];
FILE *matches_file;
FILE *requested_file;
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
            read_count = read(
                connection_file_descriptor,
                receive_buffer,
                127
            );
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
        /* FETCH PHASE: read the exact file name the client wants */
        read_count = read(
            connection_file_descriptor,
            receive_buffer,
            127
        );
        if (read_count <= 0)
        {
            close(connection_file_descriptor);
            printf("\nClient connection closed.\n");
            continue;
        }
        receive_buffer[read_count] = '\0';
        printf("\nClient requested file: %s\n", receive_buffer);
        /* Open the requested file */
        requested_file = fopen(receive_buffer, "r");
        if (requested_file == NULL)
        {
            write(connection_file_descriptor, "FILE_ERROR", strlen("FILE_ERROR"));
            printf("Cannot open requested file\n");
            close(connection_file_descriptor);
            printf("\nClient connection closed.\n");
            continue;
        }
        /* Send the whole file, then close so the client sees end of file */
        while ((file_read_count = fread(file_buffer, 1, sizeof(file_buffer), requested_file)) > 0)
        {
            write_count = write(
                connection_file_descriptor,
                file_buffer,
                file_read_count
            );
            if (write_count < 0)
            {
                printf("\nERROR: Cannot send file contents\n");
                break;
            }
        }
        fclose(requested_file);
        printf("File sent to client\n");
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

Client search query: xyz
No matching files for this query

Client search query: ab
Sent 3 matching file name(s)

Client requested file: abcd.txt
File sent to client

Client connection closed.

Server: Waiting for client...
^C
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
struct sockaddr_in server_address, client_address;
int socket_file_descriptor;
int receive_count, send_count;
int bind_result;
int client_address_length;
int match_count, file_read_count;
int mistakes;
unsigned short server_port = 25025;
char server_ip[] = "192.168.24.44";
char receive_buffer[128];
char command_buffer[256];
char list_buffer[1024];
char line_buffer[256];
char file_buffer[4096];
char perm_string[128];
char *file_name_pointer;
FILE *matches_file;
FILE *requested_file;
FILE *perm_file;
/* Send a text reply to whoever sent the last datagram */
void send_reply(char *message)
{
    sendto(socket_file_descriptor, message, strlen(message), 0,
           (struct sockaddr *)&client_address, client_address_length);
}
void main()
{
    /* Filling server address structure */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_aton(server_ip, &server_address.sin_addr);
    printf("\nUDP FILE SEARCH SERVER\n");
    /* Creating datagram socket */
    socket_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_file_descriptor < 0)
    {
        printf("\nSERVER ERROR: Cannot create socket\n");
        exit(1);
    }
    /* Binding server socket address */
    bind_result = bind(socket_file_descriptor,
                       (struct sockaddr *)&server_address,
                       sizeof(server_address));
    if (bind_result < 0)
    {
        printf("\nSERVER ERROR: Cannot bind\n");
        close(socket_file_descriptor);
        exit(1);
    }
    /* Keep serving one search session after another */
    for (;;)
    {
        printf("\nServer: Waiting for search query...\n");
        /* SEARCH PHASE: keep taking queries until a match is found */
        for (;;)
        {
            client_address_length = sizeof(client_address);
            receive_count = recvfrom(socket_file_descriptor, receive_buffer, 127, 0,
                                     (struct sockaddr *)&client_address,
                                     &client_address_length);
            if (receive_count < 0)
            {
                printf("\nSERVER ERROR: Cannot receive\n");
                continue;
            }
            receive_buffer[receive_count] = '\0';
            printf("\nClient %s search query: %s\n",
                   inet_ntoa(client_address.sin_addr), receive_buffer);
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
                send_reply("NO_RESULTS");
                printf("No matching files for this query\n");
                continue;
            }
            /* Match found: send the list and move to fetch phase */
            send_reply(list_buffer);
            printf("Sent %d matching file name(s)\n", match_count);
            break;
        }
        /* FETCH PHASE: two wrong picks allowed, then send the valid list */
        mistakes = 0;
        for (;;)
        {
            client_address_length = sizeof(client_address);
            receive_count = recvfrom(socket_file_descriptor, receive_buffer, 127, 0,
                                     (struct sockaddr *)&client_address,
                                     &client_address_length);
            if (receive_count < 0)
            {
                printf("\nSERVER ERROR: Cannot receive\n");
                continue;
            }
            receive_buffer[receive_count] = '\0';
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
                    send_reply("ERROR: no such file, enter again");
                    continue;
                }
            }
            else if (perm_string[0] == 'd')
            {
                mistakes++;
                if (mistakes < 2)
                {
                    send_reply("ERROR: this is a directory, cannot read");
                    continue;
                }
            }
            else if (perm_string[3] == 'x' || perm_string[6] == 'x' || perm_string[9] == 'x')
            {
                mistakes++;
                if (mistakes < 2)
                {
                    send_reply("ERROR: this is an executable, cannot read");
                    continue;
                }
            }
            else
            {
                /* Valid regular, non-executable file: send it and finish */
                requested_file = fopen(receive_buffer, "r");
                if (requested_file == NULL)
                {
                    send_reply("ERROR: cannot open file, enter again");
                    continue;
                }
                /* Whole file goes out as one datagram */
                file_read_count = fread(file_buffer, 1, sizeof(file_buffer), requested_file);
                fclose(requested_file);
                sendto(socket_file_descriptor, file_buffer, file_read_count, 0,
                       (struct sockaddr *)&client_address, client_address_length);
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
            send_reply(list_buffer);
            printf("Two wrong picks: sent valid file list\n");
        }
    }
}

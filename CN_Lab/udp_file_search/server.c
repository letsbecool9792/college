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
unsigned short server_port = 25025;
char server_ip[] = "192.168.24.44";
char receive_buffer[128];
char command_buffer[256];
char list_buffer[1024];
char line_buffer[256];
char file_buffer[4096];
FILE *matches_file;
FILE *requested_file;
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
            receive_count = recvfrom(
                socket_file_descriptor,
                receive_buffer,
                127,
                0,
                (struct sockaddr *)&client_address,
                &client_address_length
            );
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
                sendto(socket_file_descriptor, "NO_RESULTS", strlen("NO_RESULTS"), 0,
                       (struct sockaddr *)&client_address, client_address_length);
                printf("No matching files for this query\n");
                continue;
            }
            /* Match found: send the list and move to fetch phase */
            sendto(socket_file_descriptor, list_buffer, strlen(list_buffer), 0,
                   (struct sockaddr *)&client_address, client_address_length);
            printf("Sent %d matching file name(s)\n", match_count);
            break;
        }
        /* FETCH PHASE: read the exact file name the client wants */
        client_address_length = sizeof(client_address);
        receive_count = recvfrom(
            socket_file_descriptor,
            receive_buffer,
            127,
            0,
            (struct sockaddr *)&client_address,
            &client_address_length
        );
        if (receive_count < 0)
        {
            printf("\nSERVER ERROR: Cannot receive\n");
            continue;
        }
        receive_buffer[receive_count] = '\0';
        printf("\nClient requested file: %s\n", receive_buffer);
        /* Open the requested file */
        requested_file = fopen(receive_buffer, "r");
        if (requested_file == NULL)
        {
            sendto(socket_file_descriptor, "FILE_ERROR", strlen("FILE_ERROR"), 0,
                   (struct sockaddr *)&client_address, client_address_length);
            printf("Cannot open requested file\n");
            continue;
        }
        /* Read the whole file and send it as one datagram */
        file_read_count = fread(file_buffer, 1, sizeof(file_buffer), requested_file);
        fclose(requested_file);
        send_count = sendto(
            socket_file_descriptor,
            file_buffer,
            file_read_count,
            0,
            (struct sockaddr *)&client_address,
            client_address_length
        );
        if (send_count < 0)
            printf("\nERROR: Cannot send file contents\n");
        else
            printf("File sent to client\n");
    }
}

/*
OUTPUT
------
(server folder before running: a  ab  abc/  abcd.txt  hello.txt  server  server.c)

~/server$ ./server

UDP FILE SEARCH SERVER

Server: Waiting for search query...

Client 192.168.24.44 search query: xyz
No matching files for this query

Client 192.168.24.44 search query: ab
Sent 3 matching file name(s)

Client requested file: abcd.txt
File sent to client

Server: Waiting for search query...
^C
*/

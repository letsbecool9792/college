#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
struct sockaddr_in server_address;
int socket_file_descriptor;
int receive_count, send_count;
unsigned short server_port = 25025;
char server_ip[] = "192.168.24.44";
char send_buffer[128];
char receive_buffer[4096];
char requested_filename[128];
FILE *output_file;
void main()
{
    /* Creating datagram socket */
    socket_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_file_descriptor < 0)
    {
        printf("\nCannot create socket: Client Error\n");
        exit(1);
    }
    /* Filling server socket address with values */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_aton(server_ip, &server_address.sin_addr);
    printf("\nUDP FILE SEARCH CLIENT\n");
    /* SEARCH PHASE: send queries until the server returns matches */
    for (;;)
    {
        printf("\nEnter search query: ");
        fgets(send_buffer, 128, stdin);
        send_buffer[strcspn(send_buffer, "\n")] = '\0';
        sendto(socket_file_descriptor, send_buffer, strlen(send_buffer), 0,
               (struct sockaddr *)&server_address, sizeof(server_address));
        receive_count = recvfrom(socket_file_descriptor, receive_buffer, 4095, 0, NULL, NULL);
        if (receive_count < 0)
        {
            printf("\nClient Error: Cannot receive result\n");
            close(socket_file_descriptor);
            exit(1);
        }
        receive_buffer[receive_count] = '\0';
        /* No results: loop back and ask again */
        if (strcmp(receive_buffer, "NO_RESULTS") == 0)
        {
            printf("No results, re-enter search query\n");
            continue;
        }
        /* Matches found: show them and move on */
        printf("\nMatching files:\n%s", receive_buffer);
        break;
    }
    /* FETCH PHASE */
    for (;;)
    {
        printf("\nEnter exact file name: ");
        fgets(requested_filename, 128, stdin);
        requested_filename[strcspn(requested_filename, "\n")] = '\0';
        sendto(socket_file_descriptor, requested_filename, strlen(requested_filename), 0,
               (struct sockaddr *)&server_address, sizeof(server_address));
        receive_count = recvfrom(socket_file_descriptor, receive_buffer, 4095, 0, NULL, NULL);
        if (receive_count < 0)
        {
            printf("\nClient Error: Cannot receive reply\n");
            break;
        }
        receive_buffer[receive_count] = '\0';
        /* Wrong pick: server wants another name */
        if (strncmp(receive_buffer, "ERROR:", 6) == 0)
        {
            printf("Server: %s\n", receive_buffer);
            continue;
        }
        /* Two wrong picks used up: choose from the valid list */
        if (strncmp(receive_buffer, "VALID_LIST", 10) == 0)
        {
            printf("\nTwo wrong picks. Valid files to choose from:\n");
            printf("%s", receive_buffer + 11);   /* skip "VALID_LIST\n" */
            continue;
        }
        /* Otherwise it's the whole file in one datagram: display + save */
        output_file = fopen(requested_filename, "w");
        if (output_file == NULL)
        {
            printf("\nClient Error: cannot create local file\n");
            break;
        }
        printf("\nFile contents: %s", receive_buffer);
        fwrite(receive_buffer, 1, receive_count, output_file);
        fclose(output_file);
        printf("\nSaved a copy as %s in the client folder\n", requested_filename);
        break;
    }
    close(socket_file_descriptor);
}

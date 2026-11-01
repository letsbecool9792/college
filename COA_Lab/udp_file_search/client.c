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
        /* Send query to server */
        send_count = sendto(
            socket_file_descriptor,
            send_buffer,
            strlen(send_buffer),
            0,
            (struct sockaddr *)&server_address,
            sizeof(server_address)
        );
        if (send_count < 0)
        {
            printf("\nClient Error: Cannot send query\n");
            close(socket_file_descriptor);
            exit(1);
        }
        /* Receive search result from server */
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
    /* FETCH PHASE: send the exact file name */
    printf("\nEnter exact file name: ");
    fgets(requested_filename, 128, stdin);
    requested_filename[strcspn(requested_filename, "\n")] = '\0';
    send_count = sendto(
        socket_file_descriptor,
        requested_filename,
        strlen(requested_filename),
        0,
        (struct sockaddr *)&server_address,
        sizeof(server_address)
    );
    if (send_count < 0)
    {
        printf("\nClient Error: Cannot send file name\n");
        close(socket_file_descriptor);
        exit(1);
    }
    /* Whole file arrives as one datagram */
    receive_count = recvfrom(socket_file_descriptor, receive_buffer, 4095, 0, NULL, NULL);
    if (receive_count < 0)
    {
        printf("\nClient Error: Cannot receive file\n");
        close(socket_file_descriptor);
        exit(1);
    }
    receive_buffer[receive_count] = '\0';
    /* Server sends this exact word if it could not open the file */
    if (strcmp(receive_buffer, "FILE_ERROR") == 0)
    {
        printf("\nServer: file could not be opened\n");
        close(socket_file_descriptor);
        exit(1);
    }
    printf("\nFile contents: %s", receive_buffer);
    /* Create the same file locally in the client folder */
    output_file = fopen(requested_filename, "w");
    if (output_file == NULL)
    {
        printf("\nClient Error: Cannot create local file\n");
        close(socket_file_descriptor);
        exit(1);
    }
    fwrite(receive_buffer, 1, receive_count, output_file);
    fclose(output_file);
    printf("\nSaved a copy as %s in the client folder\n", requested_filename);
    /* Close socket */
    close(socket_file_descriptor);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
struct sockaddr_in server_address;
int socket_file_descriptor;
int read_count, write_count;
int connect_result;
int first_chunk;
unsigned short server_port = 25025;
char server_ip[] = "192.168.24.44";
char send_buffer[128];
char receive_buffer[1024];
char requested_filename[128];
FILE *output_file;
void main()
{
    /* Creating socket */
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor < 0)
    {
        printf("\nCannot create socket: Client Error\n");
        exit(1);
    }
    /* Filling server socket address with values */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_aton(server_ip, &server_address.sin_addr);
    printf("\nTCP FILE SEARCH CLIENT\n");
    /* Requesting server for a connection */
    connect_result = connect(
        socket_file_descriptor,
        (struct sockaddr *)&server_address,
        sizeof(server_address)
    );
    if (connect_result < 0)
    {
        printf("\nClient Error: Cannot connect to server\n");
        close(socket_file_descriptor);
        exit(1);
    }
    printf("\nClient: Connected to server\n");
    /* SEARCH PHASE: send queries until the server returns matches */
    for (;;)
    {
        printf("\nEnter search query: ");
        fgets(send_buffer, 128, stdin);
        send_buffer[strcspn(send_buffer, "\n")] = '\0';
        /* Send query to server */
        write_count = write(
            socket_file_descriptor,
            send_buffer,
            strlen(send_buffer)
        );
        if (write_count < 0)
        {
            printf("\nClient Error: Cannot send query\n");
            close(socket_file_descriptor);
            exit(1);
        }
        /* Receive search result from server */
        read_count = read(
            socket_file_descriptor,
            receive_buffer,
            1023
        );
        if (read_count <= 0)
        {
            printf("\nServer disconnected\n");
            close(socket_file_descriptor);
            exit(1);
        }
        receive_buffer[read_count] = '\0';
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
    write_count = write(
        socket_file_descriptor,
        requested_filename,
        strlen(requested_filename)
    );
    if (write_count < 0)
    {
        printf("\nClient Error: Cannot send file name\n");
        close(socket_file_descriptor);
        exit(1);
    }
    /* Receive file contents until server closes (end of file) */
    first_chunk = 1;
    for (;;)
    {
        read_count = read(
            socket_file_descriptor,
            receive_buffer,
            1023
        );
        if (read_count <= 0)
        {
            break;
        }
        receive_buffer[read_count] = '\0';
        if (first_chunk == 1)
        {
            /* Server sends this exact word if it could not open the file */
            if (strcmp(receive_buffer, "FILE_ERROR") == 0)
            {
                printf("\nServer: file could not be opened\n");
                break;
            }
            /* Create the same file locally in the client folder */
            output_file = fopen(requested_filename, "w");
            if (output_file == NULL)
            {
                printf("\nClient Error: Cannot create local file\n");
                break;
            }
            printf("\nFile contents: ");
            first_chunk = 0;
        }
        /* Print to screen and write to the local copy */
        printf("%s", receive_buffer);
        fwrite(receive_buffer, 1, read_count, output_file);
    }
    if (first_chunk == 0)
    {
        fclose(output_file);
        printf("\nSaved a copy as %s in the client folder\n", requested_filename);
    }
    /* Close connection */
    close(socket_file_descriptor);
}

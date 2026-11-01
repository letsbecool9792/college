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
        write(socket_file_descriptor, send_buffer, strlen(send_buffer));
        read_count = read(socket_file_descriptor, receive_buffer, 1023);
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
    /* FETCH PHASE */
    for (;;)
    {
        printf("\nEnter exact file name: ");
        fgets(requested_filename, 128, stdin);
        requested_filename[strcspn(requested_filename, "\n")] = '\0';
        write(socket_file_descriptor, requested_filename, strlen(requested_filename));
        read_count = read(socket_file_descriptor, receive_buffer, 1023);
        if (read_count <= 0)
        {
            printf("\nServer disconnected\n");
            break;
        }
        receive_buffer[read_count] = '\0';
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
        /* Otherwise it's file content: display + save a local copy */
        output_file = fopen(requested_filename, "w");
        if (output_file == NULL)
        {
            printf("\nClient Error: cannot create local file\n");
            break;
        }
        printf("\nFile contents: %s", receive_buffer);
        fwrite(receive_buffer, 1, read_count, output_file);
        for (;;)   /* keep reading until server closes (handles larger files) */
        {
            read_count = read(socket_file_descriptor, receive_buffer, 1023);
            if (read_count <= 0) break;
            receive_buffer[read_count] = '\0';
            printf("%s", receive_buffer);
            fwrite(receive_buffer, 1, read_count, output_file);
        }
        fclose(output_file);
        printf("\nSaved a copy as %s in the client folder\n", requested_filename);
        break;
    }
    close(socket_file_descriptor);
}

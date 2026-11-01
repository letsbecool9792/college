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
char receive_buffer[128];
char send_buffer[128];
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
    printf("\nTCP COMMAND CLIENT\n");
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
    /* Send commands to server */
    for (;;)
    {
        /* Get command from user */
        printf("\nEnter command: ");
        fgets(send_buffer, 128, stdin);
        /* Remove newline */
        send_buffer[strcspn(send_buffer, "\n")] = '\0';
        /* Send command to server */
        write_count = write(
            socket_file_descriptor,
            send_buffer,
            strlen(send_buffer)
        );
        if (write_count < 0)
        {
            printf("\nClient Error: Cannot send command to server\n");
            break;
        }
        /* Check if client said bye */
        if (strcmp(send_buffer, "bye") == 0)
        {
            printf("\nClient ended the session.\n");
            break;
        }
        /* Receive output from server */
        read_count = read(
            socket_file_descriptor,
            receive_buffer,
            127
        );
        if (read_count <= 0)
        {
            printf("\nServer disconnected\n");
            break;
        }
        receive_buffer[read_count] = '\0';
        printf("\n--- Server output ---\n%s\n---------------------\n", receive_buffer);
    }
    /* Close connection */
    close(socket_file_descriptor);
}

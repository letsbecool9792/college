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
char receive_buffer[128];
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
    printf("\nTCP COUNT CLIENT\n");
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
    printf("\nType your message. Send * on its own line to finish.\n");
    /* SEND PHASE: keep sending lines until user types "*" */
    for (;;)
    {
        printf("Client: ");
        fgets(send_buffer, 128, stdin);
        send_buffer[strcspn(send_buffer, "\n")] = '\0';
        write(socket_file_descriptor, send_buffer, strlen(send_buffer));
        /* Stop signal */
        if (strcmp(send_buffer, "*") == 0)
            break;
    }
    /* Receive the counts from server */
    read_count = read(socket_file_descriptor, receive_buffer, 127);
    if (read_count <= 0)
    {
        printf("\nServer disconnected\n");
        close(socket_file_descriptor);
        exit(1);
    }
    receive_buffer[read_count] = '\0';
    printf("\nServer: %s\n", receive_buffer);
    close(socket_file_descriptor);
}

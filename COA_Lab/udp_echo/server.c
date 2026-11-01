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
unsigned short server_port = 25025;
char server_ip[] = "192.168.24.44";
char receive_buffer[128];
void main()
{
    /* Filling server address structure */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_aton(server_ip, &server_address.sin_addr);
    printf("\nUDP ECHO SERVER\n");
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
    printf("\nServer: Waiting for messages...\n");
    /* No connection in UDP: just keep receiving datagrams */
    for (;;)
    {
        /* Receive message, recvfrom also fills in who sent it */
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
        printf("\nClient %s: %s\n",
               inet_ntoa(client_address.sin_addr), receive_buffer);
        /* Send the same message back to that client */
        send_count = sendto(
            socket_file_descriptor,
            receive_buffer,
            strlen(receive_buffer),
            0,
            (struct sockaddr *)&client_address,
            client_address_length
        );
        if (send_count < 0)
        {
            printf("\nERROR: Cannot echo message\n");
            continue;
        }
        printf("Echoed: %s\n", receive_buffer);
    }
}

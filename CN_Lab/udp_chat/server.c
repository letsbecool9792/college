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
char send_buffer[128];
void main()
{
    /* Filling server address structure */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_aton(server_ip, &server_address.sin_addr);
    printf("\nUDP CHAT SERVER\n");
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
    for (;;)
    {
        /* Receive message from client */
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
        /* Client said bye: don't reply, wait for the next message */
        if (strcmp(receive_buffer, "bye") == 0)
        {
            printf("\nClient ended the chat.\n");
            printf("\nServer: Waiting for messages...\n");
            continue;
        }
        /* Get message from server */
        printf("Server: ");
        fgets(send_buffer, 128, stdin);
        /* Remove newline */
        send_buffer[strcspn(send_buffer, "\n")] = '\0';
        /* Send reply to the same client */
        send_count = sendto(
            socket_file_descriptor,
            send_buffer,
            strlen(send_buffer),
            0,
            (struct sockaddr *)&client_address,
            client_address_length
        );
        if (send_count < 0)
        {
            printf("\nERROR: Cannot send message\n");
            continue;
        }
        /* Check if server said bye */
        if (strcmp(send_buffer, "bye") == 0)
        {
            printf("\nServer ended the chat.\n");
            printf("\nServer: Waiting for messages...\n");
        }
    }
}

/*
OUTPUT
------
~/server$ ./server

UDP CHAT SERVER

Server: Waiting for messages...

Client 192.168.24.44: hello
Server: hi, how are you?

Client 192.168.24.44: i am good, just testing the chat
Server: bye

Server ended the chat.

Server: Waiting for messages...
^C
*/

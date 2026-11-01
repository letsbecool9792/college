#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
struct sockaddr_in server_address, client_address;
int listen_file_descriptor, connection_file_descriptor;
int read_count, write_count;
int bind_result, listen_result;
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
    printf("\nTCP CHAT SERVER\n");
    /* Creating socket */
    listen_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_file_descriptor < 0)
    {
        printf("\nSERVER ERROR: Cannot create socket\n");
        exit(1);
    }
    /* Binding server socket address */
    bind_result = bind(listen_file_descriptor,
                       (struct sockaddr *)&server_address,
                       sizeof(server_address));
    if (bind_result < 0)
    {
        printf("\nSERVER ERROR: Cannot bind\n");
        close(listen_file_descriptor);
        exit(1);
    }
    /* Listening for client connection requests */
    listen_result = listen(listen_file_descriptor, 5);
    if (listen_result < 0)
    {
        printf("\nSERVER ERROR: Cannot listen\n");
        close(listen_file_descriptor);
        exit(1);
    }
    client_address_length = sizeof(client_address);
    /* Keep accepting new clients */
    for (;;)
    {
        printf("\nServer: Waiting for client...\n");
        /* Accept client connection */
        connection_file_descriptor = accept(
            listen_file_descriptor,
            (struct sockaddr *)&client_address,
            &client_address_length
        );
        if (connection_file_descriptor < 0)
        {
            printf("\nCannot accept client connection\n");
            continue;
        }
        printf("\nConnection from client %s accepted\n",
               inet_ntoa(client_address.sin_addr));
        /* Chat with current client */
        for (;;)
        {
            /* Receive message from client */
            read_count = read(
                connection_file_descriptor,
                receive_buffer,
                127
            );
            if (read_count <= 0)
            {
                printf("\nClient disconnected\n");
                break;
            }
            receive_buffer[read_count] = '\0';
            printf("\nClient: %s\n", receive_buffer);
            /* Check if client said bye */
            if (strcmp(receive_buffer, "bye") == 0)
            {
                printf("\nClient ended the chat.\n");
                break;
            }
            /* Get message from server */
            printf("Server: ");
            fgets(send_buffer, 128, stdin);
            /* Remove newline */
            send_buffer[strcspn(send_buffer, "\n")] = '\0';
            /* Send message to client */
            write_count = write(
                connection_file_descriptor,
                send_buffer,
                strlen(send_buffer)
            );
            if (write_count < 0)
            {
                printf("\nERROR: Cannot send message\n");
                break;
            }
            /* Check if server said bye */
            if (strcmp(send_buffer, "bye") == 0)
            {
                printf("\nServer ended the chat.\n");
                break;
            }
        }
        /* Close current client connection */
        close(connection_file_descriptor);
        printf("\nClient connection closed.\n");
    }
}

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
int line_count, word_count, char_count;
int i, in_word;
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
    printf("\nTCP COUNT SERVER\n");
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
        /* Reset running totals for this client */
        line_count = 0;
        word_count = 0;
        char_count = 0;
        /* RECEIVE PHASE: count each line as it arrives, until "*" */
        for (;;)
        {
            read_count = read(connection_file_descriptor, receive_buffer, 127);
            if (read_count <= 0)
            {
                printf("\nClient disconnected\n");
                break;
            }
            receive_buffer[read_count] = '\0';
            /* Stop signal */
            if (strcmp(receive_buffer, "*") == 0)
            {
                printf("\nReceived '*', sending totals\n");
                break;
            }
            printf("Client line: %s\n", receive_buffer);
            /* Count this one line and add to the totals */
            line_count++;
            in_word = 0;
            for (i = 0; receive_buffer[i] != '\0'; i++)
            {
                char_count++;
                if (receive_buffer[i] == ' ' || receive_buffer[i] == '\t')
                    in_word = 0;
                else if (in_word == 0)
                {
                    in_word = 1;
                    word_count++;
                }
            }
        }
        /* If client left without sending '*', skip sending */
        if (read_count <= 0)
        {
            close(connection_file_descriptor);
            printf("\nClient connection closed.\n");
            continue;
        }
        /* Send the totals back */
        sprintf(send_buffer, "Lines: %d, Words: %d, Characters: %d\n",
                line_count, word_count, char_count);
        write_count = write(connection_file_descriptor, send_buffer, strlen(send_buffer));
        if (write_count < 0)
            printf("\nERROR: Cannot send counts\n");
        else
            printf("Sent: %s", send_buffer);
        /* Close current client connection */
        close(connection_file_descriptor);
        printf("\nClient connection closed.\n");
    }
}

/*
OUTPUT
------
~/server$ ./server

TCP COUNT SERVER

Server: Waiting for client...

Connection from client 192.168.24.44 accepted
Client line: hello world
Client line: this is a test
Client line: bye

Received '*', sending totals
Sent: Lines: 3, Words: 7, Characters: 28

Client connection closed.

Server: Waiting for client...
^C
*/

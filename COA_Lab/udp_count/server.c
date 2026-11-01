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
    printf("\nUDP COUNT SERVER\n");
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
    /* Start with zero totals */
    line_count = 0;
    word_count = 0;
    char_count = 0;
    printf("\nServer: Waiting for lines...\n");
    /* Each datagram is one line, until "*" arrives */
    for (;;)
    {
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
        /* Stop signal: send totals back, then reset for the next client */
        if (strcmp(receive_buffer, "*") == 0)
        {
            printf("\nReceived '*', sending totals\n");
            sprintf(send_buffer, "Lines: %d, Words: %d, Characters: %d\n",
                    line_count, word_count, char_count);
            send_count = sendto(
                socket_file_descriptor,
                send_buffer,
                strlen(send_buffer),
                0,
                (struct sockaddr *)&client_address,
                client_address_length
            );
            if (send_count < 0)
                printf("\nERROR: Cannot send counts\n");
            else
                printf("Sent: %s", send_buffer);
            line_count = 0;
            word_count = 0;
            char_count = 0;
            printf("\nServer: Waiting for lines...\n");
            continue;
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
}

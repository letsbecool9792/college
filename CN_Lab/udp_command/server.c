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
int system_result, file_read_count;
unsigned short server_port = 25025;
char server_ip[] = "192.168.24.44";
char receive_buffer[128];
char command_buffer[256];
char output_buffer[4096];
FILE *output_file;
void main()
{
    /* Filling server address structure */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_aton(server_ip, &server_address.sin_addr);
    printf("\nUDP COMMAND SERVER\n");
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
    printf("\nServer: Waiting for commands...\n");
    for (;;)
    {
        /* Receive command from client */
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
        printf("\nClient %s command: %s\n",
               inet_ntoa(client_address.sin_addr), receive_buffer);
        /* Client said bye: nothing to run */
        if (strcmp(receive_buffer, "bye") == 0)
        {
            printf("\nClient ended the session.\n");
            continue;
        }
        /* Build command with redirection to output.txt */
        strcpy(command_buffer, receive_buffer);
        strcat(command_buffer, " > output.txt 2>&1");
        /* Run the command */
        system_result = system(command_buffer);
        /* Check system() result */
        if (system_result != 0)
        {
            /* Command did not run properly, returns a high value */
            strcpy(output_buffer, "ERROR: Command failed to run\n");
            printf("\nsystem() returned %d (command failed)\n", system_result);
        }
        else
        {
            printf("\nsystem() returned %d (command ran)\n", system_result);
            /* Read the whole of output.txt into one buffer */
            output_buffer[0] = '\0';
            output_file = fopen("output.txt", "r");
            if (output_file == NULL)
            {
                strcpy(output_buffer, "ERROR: Cannot open output file\n");
            }
            else
            {
                file_read_count = fread(output_buffer, 1, 4095, output_file);
                output_buffer[file_read_count] = '\0';
                fclose(output_file);
            }
            /* Command ran but printed nothing (e.g. touch, mkdir) */
            if (output_buffer[0] == '\0')
                strcpy(output_buffer, "(no output)\n");
        }
        /* Send the whole output back as one datagram */
        send_count = sendto(
            socket_file_descriptor,
            output_buffer,
            strlen(output_buffer),
            0,
            (struct sockaddr *)&client_address,
            client_address_length
        );
        if (send_count < 0)
            printf("\nERROR: Cannot send output\n");
    }
}

/*
OUTPUT
------
~/server$ ./server

UDP COMMAND SERVER

Server: Waiting for commands...

Client 192.168.24.44 command: ls

system() returned 0 (command ran)

Client 192.168.24.44 command: pwd

system() returned 0 (command ran)

Client 192.168.24.44 command: x

system() returned 32512 (command failed)

Client 192.168.24.44 command: touch newfile

system() returned 0 (command ran)

Client 192.168.24.44 command: ls

system() returned 0 (command ran)

Client 192.168.24.44 command: bye

Client ended the session.
^C
*/

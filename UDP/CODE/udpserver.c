#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    socklen_t client_struct_length = sizeof(client_addr);

    // Clean buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create a UDP socket
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (socket_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully!\n");

    // Set port and IP
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind the socket
    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Could not bind to the port\n");
        return -1;
    }
    printf("Done with binding!\n");
    printf("Listening for incoming messages...\n");

    // Receive client's message
    if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
                 (struct sockaddr*)&client_addr, &client_struct_length) < 0) {
        printf("Couldn't receive\n");
        return -1;
    }
    printf("Received message from IP: %s and port: %i\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("Msg from client: %s\n", client_message);

    // Get input from the server user
    printf("Enter a message to send back to the client: ");
    fgets(server_message, sizeof(server_message), stdin);
    server_message[strcspn(server_message, "\n")] = 0;

    // Respond to the client
    if (sendto(socket_desc, server_message, strlen(server_message), 0,
               (struct sockaddr*)&client_addr, client_struct_length) < 0) {
        printf("Can't send\n");
        return -1;
    }

    printf("Message sent to client: %s\n", server_message);

    // Close the socket
    close(socket_desc);

    return 0;
}


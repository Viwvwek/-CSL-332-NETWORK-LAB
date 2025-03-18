#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);
    
    // Clean buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    // Create socket
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if (socket_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set server address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Get input from the user
    printf("Enter a message: ");
    fgets(client_message, sizeof(client_message), stdin);
    client_message[strcspn(client_message, "\n")] = 0;  // Remove newline character from fgets
    
    // Send the message to the server
    if (sendto(socket_desc, client_message, strlen(client_message), 0, 
               (struct sockaddr*)&server_addr, server_struct_length) < 0) {
        printf("Error while sending message to server\n");
        return -1;
    }
    
    // Receive the server's response
    if (recvfrom(socket_desc, server_message, sizeof(server_message), 0, 
                 (struct sockaddr*)&server_addr, &server_struct_length) < 0) {
        printf("Error while receiving server response\n");
        return -1;
    }
    
    // Print server response
    printf("Server response: %s\n", server_message);
    
    // Close the socket
    close(socket_desc);

    return 0;
}


#include <stdio.h>
#include <string.h>
#include <sys/socket.h> // Socket programming functions
#include <arpa/inet.h>  // Definitions for internet operations
#include <unistd.h>     // UNIX standard functions (for close)

int main(void) {

    int k=5,m=1,p;
    char buffer[1024];
    // Declaration of variables
    int socket_desc; // Socket descriptor for the client
    struct sockaddr_in server_addr; // Structure for server address
    char server_message[2000], client_message[2000]; // Buffers for messages

    // Clean buffer
    memset(server_message, '\0', sizeof(server_message)); // Initialize server message buffer to zero
    memset(client_message, '\0', sizeof(client_message)); // Initialize client message buffer to zero

    // Create the socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket
    if (socket_desc < 0) { // Check if socket creation failed
        printf("Socket not created\n");
        return -1; // Exit if socket creation fails
    }
    printf("Socket created successfully\n");

    // Server and port configuration
    server_addr.sin_family = AF_INET; // Use IPv4
    server_addr.sin_port = htons(2000); // Set port number (2000) and convert to network byte order
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set IP address to localhost

    // Connect to the server
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect!\n");
        return -1; // Exit if connection fails
    }
    printf("Connected to server successfully!\n");

    while(k!=0){
        if(m<=5){
            printf("Sending frame!\n");
        }
        if(m%2==0)
        {
            printf("sending is successfull!\n");
        }
        else{
            strcpy(buffer,"error");
            printf("packet loss!\n");
            for(p=1;p<=3;p++)
            {
                printf("Waiting for %d seconds\n",p);
            }
            printf("Retransmitting.....\n");
            strcpy(buffer,"frame");
            sleep(3);
        }

        int y = send(socket_desc,buffer,19,0);
        if(y==-1)
        {
            printf("Error in sending");
            exit(1);
        }
        else
        {
            printf("Sent frame %d\n",m);

        }
        int z = recv(socket_desc,buffer,1024,0);
        if(z==-1)
        {
            printf("Error in Receiving data: ");
        }
        k--;
        m++;

    }
    close(socket_desc);
    return 0;


}

#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <unistd.h>     
#include <stdlib.h>

int main(void) {
    int frames_to_send;
    int frame_number = 1;
    char buffer[1024];

    printf("Enter total number of frames to send: ");
    scanf("%d", &frames_to_send);

    int socket_desc; 
    struct sockaddr_in server_addr; 

    socket_desc = socket(AF_INET, SOCK_STREAM, 0); 
    if (socket_desc < 0) { 
        printf("Socket not created\n");
        return -1; 
    }
    printf("Socket created successfully\n");

    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(2000); 
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect!\n");
        return -1; 
    }
    printf("Connected to server successfully!\n");

    // Send total frames to server
    send(socket_desc, &frames_to_send, sizeof(frames_to_send), 0);

    while (frames_to_send > 0) {
        printf("Sending frame %d\n", frame_number);

        if (frame_number % 2 != 0) {
            printf("Packet loss detected for frame %d\n", frame_number);
            sleep(3);
            printf("Retransmitting frame %d...\n", frame_number);
        }

        strcpy(buffer, "frame");
        if (send(socket_desc, buffer, sizeof(buffer), 0) == -1) {
            printf("Error in sending\n");
            exit(1);
        }

        printf("Frame %d sent successfully\n", frame_number);

        if (recv(socket_desc, buffer, sizeof(buffer), 0) == -1) {
            printf("Error in receiving data\n");
        } else {
            printf("Acknowledgment received: %d\n", frame_number);
        }

        frames_to_send--;
        frame_number++;
    }

    close(socket_desc);
    return 0;
}

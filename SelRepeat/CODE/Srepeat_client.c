#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000];
    
    memset(server_message, '\0', sizeof(server_message));

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        printf("Error creating socket\n");
        return -1;
    }
    printf("Socket created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error connecting to server\n");
        return -1;
    }
    printf("Connected to server\n");

    char buffer[1024];
    int total_frames = 5;
    int frame_number = 0;

    while (frame_number < total_frames) {
        printf("Sending frame %d\n", frame_number);

        if (frame_number % 2 != 0) {  
            printf("Packet loss detected for frame %d\n", frame_number);
            sleep(3);
            printf("Retransmitting frame %d...\n", frame_number);
        }

        snprintf(buffer, sizeof(buffer), "%d", frame_number);
        if (send(socket_desc, buffer, strlen(buffer), 0) < 0) {
            printf("Unable to send message\n");
            return -1;
        }

        if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
            printf("Error while receiving acknowledgment. Resending frame %d...\n", frame_number);
            continue;
        }

        printf("Acknowledgment received for frame %d\n", frame_number);
        frame_number++;
    }

    close(socket_desc);
    return 0;
}

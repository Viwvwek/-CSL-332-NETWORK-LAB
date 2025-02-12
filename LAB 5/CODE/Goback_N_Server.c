#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int connfd) {
    char buff[MAX];
    int ack = 0;
    int n;

    while (1) {
        
        sleep(1);

        bzero(buff, sizeof(buff));
        n = recv(connfd, buff, sizeof(buff), 0);
        if (n <= 0) {
            perror("Receive failed");
            break;
        }


        printf("Frame received: %s\n", buff);
        int frame_num = atoi(buff);

        if (frame_num == ack) {
            printf("Acknowledgment sent: %d\n", ack);
            snprintf(buff, sizeof(buff), "%d", ack);
            send(connfd, buff, sizeof(buff), 0);
            ack++;
        } else {
            printf("Duplicate frame received: %d\n", frame_num);

            snprintf(buff, sizeof(buff), "%d", ack - 1);
            send(connfd, buff, sizeof(buff), 0);
        }


        if (strcmp(buff, "Exit") == 0) {
            printf("Exit command received. Closing connection.\n");
            break;
        }
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    // Send initial frames

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(0);
    } else {
        printf("Socket successfully created\n");
    }


    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Socket bind failed");
        exit(0);
    } else {
        printf("Socket successfully bound\n");
    }


    if (listen(sockfd, 5) != 0) {
        perror("Listen failed");
        exit(0);
    } else {
        printf("Server listening...\n");
    }

   
    socklen_t len = sizeof(cli);
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        perror("Server accept failed");
        exit(0);
    } else {
        printf("Server accepted the client\n");
    }


    func(connfd);

    close(connfd);
    close(sockfd);
    return 0;
}

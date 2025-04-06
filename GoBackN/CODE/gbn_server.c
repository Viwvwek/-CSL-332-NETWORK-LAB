#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed\n");
        exit(0);
    }
    printf("Socket successfully created\n");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);

    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed\n");
        exit(0);
    }
    printf("Socket successfully binded\n");

    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed\n");
        exit(0);
    }
    printf("Server listening\n");

    len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr *)&cli, (socklen_t*)&len);
    if (connfd < 0) {
        printf("Server accept failed\n");
        exit(0);
    }
    printf("Server accepted the client\n");

    srand(time(NULL));
    char buffer[80];
    int frame, ack = 0, next = 0;

    while (1) {
        sleep(1);
        memset(buffer, 0, sizeof(buffer));
        recv(connfd, buffer, sizeof(buffer), 0);

        if (strcmp("Exit", buffer) == 0) {
            printf("Exit\n");
            break;
        }

        frame = atoi(buffer);
        if (frame != next) {
            printf("Frame %d discarded\nAcknowledgement sent: %d\n", frame, ack);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%d", ack);
            send(connfd, buffer, sizeof(buffer), 0);
            continue;
        }

        int decision = rand() % 3;
        if (decision == 0) {
            // Simulate frame loss (do nothing)
        } else {
            ack = frame;
            if (decision == 1)
                sleep(2); // Simulate delay
            printf("Frame %d received\nAcknowledgement sent: %d\n", frame, ack);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%d", ack);
            send(connfd, buffer, sizeof(buffer), 0);
            next = ack + 1;
        }
    }

    close(sockfd);
    return 0;
}

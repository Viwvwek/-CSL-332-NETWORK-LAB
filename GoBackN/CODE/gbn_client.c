#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd, numFrames, windowSize;
    struct sockaddr_in servaddr;
    struct timeval timeout;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed\n");
        exit(0);
    }
    printf("Socket successfully created\n");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(8080);

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed\n");
        exit(0);
    }
    printf("Connected to the server\n");

    printf("Enter the number of frames: ");
    scanf("%d", &numFrames);
    printf("Enter the window size: ");
    scanf("%d", &windowSize);

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0)
        perror("setsockopt(SO_RCVTIMEO) failed");

    char buffer[80];
    int ack, i = 0, n, w1 = 0, w2 = windowSize - 1, j, flag = 0;

    for (i = 0; i < numFrames && i <= w2; i++) {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%d", i);
        send(sockfd, buffer, sizeof(buffer), 0);
        printf("Frame %d sent\n", i);
    }

    while (1) {
        if (w2 - w1 != windowSize - 1 && flag == 0 && i != numFrames) {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%d", i);
            send(sockfd, buffer, sizeof(buffer), 0);
            printf("Frame %d sent\n", i);
            w2++;
            i++;
        }

        flag = 0;
        memset(buffer, 0, sizeof(buffer));
        n = recv(sockfd, buffer, sizeof(buffer), 0);
        ack = atoi(buffer);

        if (n > 0) {
            if (ack + 1 == numFrames) {
                printf("Acknowledgement received: %d\nAll frames sent successfully\n", ack);
                memset(buffer, 0, sizeof(buffer));
                strcpy(buffer, "Exit");
                send(sockfd, buffer, sizeof(buffer), 0);
                break;
            }

            if (ack == w1) {
                printf("Acknowledgement received: %d\n", ack);
                w1++;
            }
        } else {
            printf("Acknowledgement not received for %d\nResending frames\n", w1);
            for (j = w1; j < numFrames && j < w1 + windowSize; j++) {
                memset(buffer, 0, sizeof(buffer));
                snprintf(buffer, sizeof(buffer), "%d", j);
                send(sockfd, buffer, sizeof(buffer), 0);
                printf("Frame %d sent\n", j);
            }
            flag = 1;
        }
    }

    close(sockfd);
    return 0;
}

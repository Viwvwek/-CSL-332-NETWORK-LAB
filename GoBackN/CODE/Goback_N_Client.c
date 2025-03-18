#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

struct timeval timeout;

void func(int sockfd, int nf, int ws) {
    char buff[MAX];
    int ack, i = 0, n, k, w1 = 0, w2 = ws - 1, j, flag = 0;


    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt failed");
        return;
    }

    for (i = 0; i < nf && i <= w2; i++) {
        bzero(buff, sizeof(buff));
        snprintf(buff, sizeof(buff), "%d", i);
        k = send(sockfd, buff, sizeof(buff), 0);
        if (k < 0) {
            perror("Send failed");
            return;
        }
        printf("Frame %d sent\n", i);
    }


    while (1) {
        if (w2 - w1 != ws - 1 && flag == 0 && i != nf) {
            bzero(buff, sizeof(buff));
            snprintf(buff, sizeof(buff), "%d", i);
            k = send(sockfd, buff, sizeof(buff), 0);
            if (k < 0) {
                perror("Send failed");
                return;
            }
            printf("Frame %d sent\n", i);
            w2++;
            i++;
        }

        flag = 0;
        bzero(buff, sizeof(buff));
        n = recv(sockfd, buff, MAX, 0);
        if (n > 0) {
            ack = atoi(buff);
            if (ack + 1 == nf) {
                printf("Acknowledgment received: %d\nExit\n", ack);
                bzero(buff, sizeof(buff));
                strcpy(buff, "Exit");
                k = send(sockfd, buff, sizeof(buff), 0);
                if (k < 0) {
                    perror("Send failed");
                }
                break;
            }
            if (ack == w1) {
                w1++;
                printf("Acknowledgment received: %d\n", ack);
            }
        } else {
            printf("Acknowledgment not received for %d\nResending frames\n", w1);
            for (j = w1; j < nf && j < w1 + ws; j++) {
                bzero(buff, sizeof(buff));
                snprintf(buff, sizeof(buff), "%d", j);
                k = send(sockfd, buff, sizeof(buff), 0);
                if (k < 0) {
                    perror("Send failed");
                    return;
                }
                printf("Frame %d sent\n", j);
            }
            flag = 1;
        }
    }
}

int main() {
    int sockfd, f, w;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(0);
    } else {
        printf("Socket successfully created\n");
    }


    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT);


    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection to the server failed");
        exit(0);
    } else {
        printf("Connected to the server\n");
        printf("Enter the number of frames: ");
        scanf("%d", &f);
        printf("Enter the size of the window: ");
        scanf("%d", &w);
    }

    func(sockfd, f, w);

    close(sockfd);
}

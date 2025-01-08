#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

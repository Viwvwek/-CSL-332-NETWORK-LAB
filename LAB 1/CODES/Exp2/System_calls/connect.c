#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

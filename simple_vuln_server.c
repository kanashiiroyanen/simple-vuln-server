/* simple_vuln_server.c */

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 8888

int main(void)
{
    int sockfd;
    int new_sockfd;
    struct sockaddr_in host_addr;
    struct sockaddr_in client_addr;
    socklen_t sin_size;
    int recv_length;
    int optval = 1;
    char buf[200];

    printf("buf = %p\n", buf);

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    printf("sockfd: %d, src_port: %d\n", sockfd, PORT);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval)) == -1) {
        perror("setsockopt");
        return -1;
    }

    memset(&host_addr, 0, sizeof(struct sockaddr_in));
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    //host_addr.sin_addr.s_addr = 0;
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //bzero((char *)&host_addr, sizeof(host_addr));
    //memset(&(host_addr.sin_zero), '\0', 8);

    if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        return -1;
    }

    if (listen(sockfd, 5) == -1) {
        perror("listen");
        return -1;
    }

    sin_size = sizeof(client_addr);

    new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);

    if (new_sockfd == -1) {
        perror("accept");
        return -1;
    }

    printf("new_sockfd: %d\n", new_sockfd);
    printf("connection to server: %s, dst_port: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    send(new_sockfd, "Hello\n", 6, 0);
    printf("buf = %p\n", buf);

    recv_length = recv(new_sockfd, &buf, 1024, 0);
    printf("%s\n", buf);

    close(new_sockfd);
    close(sockfd);

    return 0;
}

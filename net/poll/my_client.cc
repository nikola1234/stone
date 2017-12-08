#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <iostream>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

#define SERVER_PORT 8787
#define MAXLINE 1024


static const std::string IP_ADDR ("127.0.0.1");

int main(void)
{
    int sock_fd;
    if ((sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        ERR_EXIT("Socket");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(PF_INET, IP_ADDR.c_str(), &serv_addr.sin_addr);

    if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        ERR_EXIT("Connect");

    struct sockaddr_in localaddr;
    socklen_t addrlen = sizeof(localaddr);
    if (getsockname(sock_fd, (struct sockaddr*)&localaddr, &addrlen) < 0)
    ERR_EXIT("Getsock_fdname");

    std::cout << "ip: "     << inet_ntoa(localaddr.sin_addr)
              << ", port: " << ntohs(localaddr.sin_port) << std::endl;

    char sendbuf[MAXLINE] = {0};
    char recvbuf[MAXLINE] ={0};
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        write(sock_fd, sendbuf, strlen(sendbuf));
        read(sock_fd, recvbuf, sizeof(recvbuf));

        std::cout <<"echo end... recive data : " << recvbuf <<std::endl;
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }

    close(sock_fd);
    
    return 0;
}

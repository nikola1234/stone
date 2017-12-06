
#include <iostream>
#include <string.h>
#include <sys/types.h>    // socket()
#include <sys/socket.h>
#include <netinet/in.h>   // struct sockaddr_in
#include <arpa/inet.h>    // inet_pton
/*
struct sockaddr_in {
    short sin_family;            // Address family一般来说AF_INET（地址族）PF_INET（协议族） 
    unsigned short sin_port;     // Port number(必须要采用网络数据格式,普通数字可以用htons()函数转换成网络数据格式的数字)
    struct in_addr sin_addr;     // IP address in network byte order（Internet address）
    unsigned char sin_zero[8];   // Same size as struct sockaddr没有实际意义,只是为了　跟SOCKADDR结构在内存中对齐
};
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

#define SERVER_PORT 8787
#define MAXLINE 1024

static const std::string IP_ADDR ("127.0.0.1");
/*
  1、获取socket描述符
  2、设置服务器网络地址
  3、连接服务器
*/
int main(int argc, char const *argv[])
{
    int sock_fd;
    struct sockaddr_in serv_addr;
    //用于根据指定的地址族、数据类型和协议来分配一个套接口的描述字及其所用的资源
    //如果协议protocol未指定（等于0），则使用缺省的连接方式。
    sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_fd < 0)
        ERR_EXIT("Socket");

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, IP_ADDR.c_str(), &serv_addr.sin_addr);

    int retval = 0;
    retval = connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (retval < 0)
        ERR_EXIT("Connect");

    struct sockaddr_in localaddr;
    socklen_t addrlen = sizeof(localaddr);
    if (getsockname(sock_fd, (struct sockaddr*)&localaddr, &addrlen) < 0)
    ERR_EXIT("Getsockname");

    std::cout << "ip: "     << inet_ntoa(localaddr.sin_addr)
              << ", port: " << ntohs(localaddr.sin_port) << std::endl;

    return 0;
}



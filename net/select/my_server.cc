
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <vector>
#include <iostream>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

#define PRINT(des, str)  \
        std::cout << des << " " << str << std::endl;

#define D_NET_LOG(format, ...) \
    printf(format , ##__VA_ARGS__) //示例：获取参数

#define SERVER_PORT 8787
#define MAXLINE 1024

//正在监听的文件描述符结构体
typedef struct Listen_fd
{
    int fd;  // 被监听的文件描述符
} L_FD;

typedef std::vector<L_FD> LisFdList;

static const std::string IP_ADDR ("127.0.0.1");

/*
  1、获取socket描述符
  2、设置服务器网络地址
  3、绑定网络地址
  4、监听文件描述符
*/
int main(int argc, char const *argv[])
{
    signal(SIGPIPE, SIG_IGN);  //屏蔽客户端关闭，服务器写数据失败的信号
    signal(SIGCHLD, SIG_IGN);  //屏蔽僵尸进程的信号

    int serv_sock_fd;
    int retval = 0;
    struct sockaddr_in serv_addr;
    serv_sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (serv_sock_fd < 0)
        ERR_EXIT("Socket");

    /*一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。*/
    int reuse = 1;
    if (setsockopt(serv_sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        ERR_EXIT("setsockopt");
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, IP_ADDR.c_str(), &serv_addr.sin_addr);

    if (bind(serv_sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        ERR_EXIT("Bind");
    }

    if (listen(serv_sock_fd, SOMAXCONN) < 0) {  //SOMAXCONN = 128
        ERR_EXIT("Listen");
    }

    //将serv_sock_fd加入到监听队列中
    LisFdList selecetfds;
    L_FD sfd;
    sfd.fd = serv_sock_fd;
    selecetfds.push_back(sfd);

    while (1)
    {
        int nfds = -1;
        fd_set readfds;
        /*每次调用select前都要重新设置文件描述符和时间，因为事件发生后，文件描述符和时间都被内核修改*/
        FD_ZERO(&readfds);
        /*添加监听套接字*/
        for (auto it : selecetfds) {
            FD_SET(it.fd, &readfds);
            nfds = it.fd > nfds ? it.fd : nfds;
        }

        retval = select(nfds + 1, &readfds, nullptr, nullptr, nullptr);
    }
    return 0;
}
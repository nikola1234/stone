
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>  // read write
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
    Listen_fd(int lfd) {fd = lfd;} 
} L_FD;

typedef std::vector<L_FD> LisFdList;

static const std::string IP_ADDR ("127.0.0.1");

void accept_client_proc(int servfd, LisFdList& selecetfds)
{
    int cli_fd = -1;
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);

    cli_fd = accept(servfd, (struct sockaddr*)&cli_addr, &cli_addr_len);
    if (cli_fd < 0) {
        ERR_EXIT("Accept");  //忽略 errno = EINTR
    }
    std::cout << "this client fd : " << cli_fd << std::endl;
    std::cout << "accept a new client: " << inet_ntoa(cli_addr.sin_addr)
              << " Port : " << ntohs(cli_addr.sin_port) << std::endl;
    L_FD lfd(cli_fd);
    selecetfds.push_back(lfd);
}

void recv_client_msg(fd_set *readfds, LisFdList& selecetfds, int nready)
{
    char buf[MAXLINE] = {0};
    for (LisFdList::iterator it = selecetfds.begin();
            it != selecetfds.end() && nready > 0; ++it) {
        int connfd = it->fd;
        /*判断客户端套接字是否有数据*/
        if(FD_ISSET(connfd, readfds)) {
            //接收客户端发送的信息
            int ret = read(connfd, buf, MAXLINE);
            if (ret == -1) {
                ERR_EXIT("Read");
            }
            if (ret == 0) { // 客户端关闭
                std::cout<<"Client close."<<std::endl;
                it = selecetfds.erase(it);
                --it;

                close(connfd);
                continue;
            }
            struct sockaddr_in localaddr;
            socklen_t addrlen = sizeof(localaddr);
            if (getsockname(connfd, (struct sockaddr*)&localaddr, &addrlen) < 0)
                ERR_EXIT("Getsockname");

            std::cout << "Recv Client(" << inet_ntoa(localaddr.sin_addr)
              << " : " << ntohs(localaddr.sin_port) <<")" << "data : " << buf << std::endl;


            write(connfd, buf, strlen(buf));
        }
    }
}
/*
  1、获取socket描述符
  2、设置服务器网络地址
  3、绑定网络地址
  4、监听文件描述符
  5、用select确定套接字的状态
     服务器的套接字是否有连接动作 ？ 如果有了，获取到客户端的套接字加入监听队列
     客户端的套接字是否有读动作 ？ 有读就执行读操作(客户端关闭，客户端的套接字会有读动作且读的数量为0，就将客户端套接字关闭，从监听队列中移除)

优点，既要处理监听套接字，又要处理已连接的套接字。不需要多余创建线程和进程处理，减少了系统开销。
*/
int main(int argc, char const *argv[])
{
    signal(SIGPIPE, SIG_IGN);  //屏蔽客户端关闭，服务器写数据失败的信号
    signal(SIGCHLD, SIG_IGN);  //屏蔽僵尸进程的信号

    LisFdList selecetfds;  // 客户端读监听队列

    int serv_sock_fd;
    int nready = 0;
    struct sockaddr_in serv_addr;
    serv_sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (serv_sock_fd < 0)
        ERR_EXIT("Socket");

    /*一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。*/
    int reuse = 1;
    if (setsockopt(serv_sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        ERR_EXIT("Setsockopt");
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

    while (1)
    {
        int nfds = -1;
        fd_set readfds;
        /*每次调用select前都要重新设置文件描述符和时间，因为事件发生后，文件描述符和时间都被内核修改*/
        FD_ZERO(&readfds);
        /*监听客户端连接*/
        FD_SET(serv_sock_fd, &readfds);
        nfds = serv_sock_fd;
        /*添加监听套接字*/
        for (auto it : selecetfds) {
            FD_SET(it.fd, &readfds);
            nfds = it.fd > nfds ? it.fd : nfds;
        }

        PRINT("Client number : ", selecetfds.size());
        nready = select(nfds + 1, &readfds, nullptr, nullptr, nullptr); // 当返回为正数时，表示已经准备好的描述符数。
        if (nready < 0) {
            ERR_EXIT("Select");
        }
        if (FD_ISSET(serv_sock_fd, &readfds)) {
            /*监听客户端请求*/
            PRINT("New Client connecting", "...");
            accept_client_proc(serv_sock_fd, selecetfds);
			nready--;
        }
		if (nready) {
            /*接受处理客户端消息*/
            PRINT("Client message coming", "...");
            recv_client_msg(&readfds, selecetfds, nready);
        }
    }
    return 0;
}
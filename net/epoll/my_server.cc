#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <vector>
#include <algorithm>
#include <iostream>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

#define SERVER_PORT 8787
#define MAXLINE 1024

#define PRINT(des, str)  \
        std::cout << des << " " << str << std::endl;

static const std::string IP_ADDR ("127.0.0.1");

typedef std::vector<struct epoll_event> EventList;

int main(int argc, char const *argv[])
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    // 文件描述符有一个属性：CLOEXEC，即当调用exec()函数成功后，文件描述符会自动关闭
    int idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC); // 空闲 fd ，在fd最大数量用完后 accept临时接收连接用的。
    if (idlefd < 0) ERR_EXIT("Idlefd");

    int sv_sk_fd;
    struct sockaddr_in serv_addr;
    sv_sk_fd = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sv_sk_fd < 0) ERR_EXIT("Socket");

    int reuse = 1;
    if (setsockopt(sv_sk_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        ERR_EXIT("Setsockopt");
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(PF_INET, IP_ADDR.c_str(), &serv_addr.sin_addr);

    if (bind(sv_sk_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        ERR_EXIT("Bind");
    }

    if (listen(sv_sk_fd, SOMAXCONN) < 0) {  //SOMAXCONN = 128
        ERR_EXIT("Listen");
    }
    // 建立一个epoll对象
    int epollfd;
    epollfd = epoll_create1(EPOLL_CLOEXEC);   //执行后关闭

    struct epoll_event event;
    event.data.fd = sv_sk_fd;
    event.events  = EPOLLIN/* | EPOLLET*/; // 默认电平触发
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sv_sk_fd, &event);

    EventList events(16);
    struct sockaddr_in peeraddr;
    socklen_t peerlen;
    int connfd;

    int nready;
    while (1) {
        nready = epoll_wait(epollfd, events.data(), static_cast<int>(events.size()), -1);
        if (nready == -1)
        {
            if (errno == EINTR)
                continue;
            
            ERR_EXIT("Epoll_wait");
        }
        if (nready == 0)    // nothing happended
            continue;
        for (int i = 0; i < nready; ++i) {
            if (events[i].data.fd == sv_sk_fd) {
                peerlen = sizeof(peeraddr);
                connfd = ::accept4(sv_sk_fd, (struct sockaddr*)&peeraddr,
                        &peerlen, SOCK_NONBLOCK | SOCK_CLOEXEC);

                if (connfd == -1) {
                    if (errno == EMFILE) {
                        close(idlefd);
                        idlefd = accept(sv_sk_fd, nullptr, nullptr);
                        close(idlefd);
                        idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);
                        continue;
                    } else {
                        ERR_EXIT("Accept4");
                    }
                }
                std::cout << "This client fd : " << connfd << std::endl;
                std::cout << "Accept a new client: " << inet_ntoa(peeraddr.sin_addr)
                          << " Port : " << ntohs(peeraddr.sin_port) << std::endl;

                struct epoll_event conn_event;
                conn_event.data.fd = connfd;
                conn_event.events = EPOLLIN;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &conn_event);   //添加客户端的监听
            } else if (events[i].events & EPOLLIN) {
                connfd = events[i].data.fd;
                if (connfd < 0)
                    continue;

                char buf[MAXLINE] = {0};
                int ret = read(connfd, buf, MAXLINE);
                if (ret == -1)
                    ERR_EXIT("Read");
                if (ret == 0)
                {
                    std::cout<<"Client close."<<std::endl;
                    close(connfd);
                    event = events[i];
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, connfd, &event);
                    //vector 删除某一个或多个相同的数据，又不用操心迭代器失效的替代方法。
                    //std::remove(clients.begin(), clients.end(), connfd),将无关数据前移，并返回有效数据最后的地址
                    //clients.erase(std::remove(clients.begin(), clients.end(), connfd), clients.end());
                    continue;
                }
                struct sockaddr_in localaddr;
                socklen_t addrlen = sizeof(localaddr);
                if (getsockname(connfd, (struct sockaddr*)&localaddr, &addrlen) < 0)
                    ERR_EXIT("Getsockname");

                std::cout << "Recv Client(" << inet_ntoa(localaddr.sin_addr)
                  << " : " << ntohs(localaddr.sin_port) <<")" << " data : " << buf;

                write(connfd, buf, strlen(buf));
            }
        }
    }
    return   0;
}
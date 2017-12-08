
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <poll.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <vector>
#include <iostream>
#include <string.h>

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

typedef std::vector<struct pollfd> PollFdList;

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

    //将服务器套接字加入到监听队列
    struct pollfd sver_fd;
    sver_fd.fd = sv_sk_fd;
    sver_fd.events = POLLIN;  // 监听POLLIN事件

    PollFdList pollfds;
    pollfds.push_back(sver_fd);

/*
    PRINT("POLLIN ", POLLIN);          1
    PRINT("POLLRDNORM ", POLLRDNORM);  64
    PRINT("POLLRDBAND ", POLLRDBAND);  128
    PRINT("POLLWRNORM ", POLLWRNORM);  256
    PRINT("POLLWRBAND ", POLLWRBAND);  512
    PRINT("POLLERR ", POLLERR);        8
    PRINT("POLLHUP ", POLLHUP);        16
    PRINT("POLLNVAL ", POLLNVAL);      32
*/
    while (1) {
        int nready = -1;   // 所有网络函数的返回参数
        int connfd = -1;     // 连接的套接字
         std::cout << "Listen fd : ";
        for (auto it : pollfds) {
            std::cout << it.fd << " ";
        }
        std::cout << std::endl;
        nready = poll(pollfds.data(), pollfds.size(), -1);
        // nready pollfds中准备好读、写或出错状态的那些socket描述符的总数量.
        if (nready < 0) { // 实际 nready == -1 poll函数调用失败，同时会自动设置全局变量errno；
            /*
             EINTR错误的产生：当阻塞于某个慢系统调用的一个进程捕获某个信号且相应信号处理函数返回时，
             该系统调用可能返回一个EINTR错误。例如：在socket服务器端，设置了信号捕获机制，有子进程，
             当在父进程阻塞于慢系统调用时由父进程捕获到了一个有效信号时，内核会致使accept返回一个EINTR错误(被中断的系统调用)
             */
            if (errno == EINTR)  // 表示中断
                continue;
            ERR_EXIT("Poll");
        }
        if (nready == 0)    // nothing happended
            continue;
        // 查看是否有新的连接
        if (pollfds[0].revents & POLLIN) {  // 用与操作判断服务器的socket是否有pollin信号，也就是连接信号
            // 将新的连接套接字加入到监听队列中
            struct sockaddr_in peeraddr;
            socklen_t peerlen = sizeof(peeraddr);
            connfd = accept4(sv_sk_fd, (struct sockaddr*)&peeraddr,
                        &peerlen, SOCK_NONBLOCK | SOCK_CLOEXEC); //用非阻塞的模式
            if (connfd == -1)
            {
                if (errno == EMFILE)  // 描述符数量用完
                {
                    close(idlefd);
                    idlefd = accept(sv_sk_fd, nullptr, nullptr);
                    // 临时处理省略
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

            struct pollfd pfd;
            pfd.fd = connfd;
            pfd.events = POLLIN;
            pfd.revents = 0;
            pollfds.push_back(pfd);

            --nready;      // 处理完了客户端连接事件，待处理事件数量减一

            if (nready  == 0) {  // 如果没有要处理的时间继续poll等待.
                continue;
            }
        }
        //查看已连接的套接字是否有读写信号
        for (PollFdList::iterator it = pollfds.begin() + 1;
            it != pollfds.end() && nready > 0; it++) {
            // 查看是哪一个套接字有读信号
            if (it->revents & POLLIN) {
                --nready; 
                char buf[MAXLINE] = {0};
                connfd = it->fd;
                int ret = read(connfd, buf, MAXLINE);
                if (ret == -1)
                    ERR_EXIT("read");
                if (ret == 0) {
                    std::cout<<"Client close."<<std::endl;
                    it = pollfds.erase(it);  // 迭代器失效
                    --it;

                    close(connfd); // bug 修正，it->fd 改为 connfd，错误关闭 sv_sk_fd， poll() 出现 POLLNVAL错误:此描述符非法
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

    return 0;
}
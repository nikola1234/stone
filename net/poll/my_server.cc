
#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

int main(int argc, char const *argv[])
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    // 文件描述符有一个属性：CLOEXEC，即当调用exec()函数成功后，文件描述符会自动关闭
    int idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC); // 空闲 fd ，在fd最大数量用完后 accept临时接收连接用的。
    if (idlefd < 0) ERR_EXIT("Idlefd");

    int sv_sk_fd;
    sv_sk_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sv_sk_fd < 0) ERR_EXIT("Socket");

    return 0;
}
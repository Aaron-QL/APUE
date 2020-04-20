#include "../apue.3e/include/apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

void dameonize(const char *cmd)
{
    pid_t pid;
    struct rlimit rl;
    int fd0, fd1, fd2;

    //1 关闭屏蔽字
    umask(0);

    //2 fork，父进程退出
    if ((pid = fork()) < 0) {
        err_sys("error");
    } else if (pid > 0) {
        exit(0);
    }

    //3 建立新会话，使调用进程成为会话首进程和组长进程，且没有控制终端
    setsid();

    //4 改变当前工作目录为根目录
    if (chdir("/") < 0) {
        err_sys("error");
    }


    // 关闭所有描述符
    if (getrlimit(RLIMIT_NOFILE, &rl) != 0) {
        err_sys("error");
    }
    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }
    for (int i = 0; i < rl.rlim_max; ++i) {
        close(i);
    }

    /*
     * Attach file descriptors 0, 1, and 2 to /dev/null.
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(fd0);
    fd2 = dup(fd0);

    // optional: Initialize the log file.
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}

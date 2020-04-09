#include "../apue.3e/include/apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

void dameonize(const char *cmd)
{
    int i, pid, fd0, fd1, fd2;
    struct rlimit rl;
    struct sigaction sa;

    //1 关闭屏蔽字
    umask(0);

    //2 获取最大描述符限制
    if (getrlimit(RLIMIT_NOFILE, &rl) != 0) {
        err_quit("%s: can't get file limit", cmd);
    }

    //3 fork，父进程退出
    if ((pid = fork()) < 0) {
        err_quit("%s: can't fork", cmd);
    } else if (pid > 0) {
        exit(0);
    }

    //4 建立新会话
    setsid();

    //5 Ensure future opens won't allocate controlling TTYs.
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        err_quit("%s: can't ignore SIGHUP", cmd);
    }

    if ((pid = fork()) < 0) {
        err_quit("%s: can't fork", cmd);
    } else if (pid > 0) {
        exit(0);
    }


    /*
     * 6 Change the current working directory to the root so
     * we won't prevent file systems from being unmounted.
     */
    if (chdir("/") < 0) {
        err_quit("%s: can't change directory to /", cmd);
    }




    /*
     * Close all open file descriptors.
     */
    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }
    for (i = 0; i < rl.rlim_max; i++) {
        close(i);
    }

    /*
     * Attach file descriptors 0, 1, and 2 to /dev/null.
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /*
     * Initialize the log file.
     */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}

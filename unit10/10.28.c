#include "../apue.3e/include/apue.h"

int my_system(char *cmd_string)
{
    int status; //执行结果
    pid_t pid; //进程id
    sigset_t child_mask, save_mask; // 屏蔽字
    struct sigaction ignore_action, save_interrupt_action, save_quit_action; // 信号处理动作

    //0. 校验命令
    if (cmd_string == NULL) {
        return -1;
    }

    //1.设置中断和退出信号的处理动作为忽略
    ignore_action.__sigaction_u = (union __sigaction_u) SIG_IGN;
    ignore_action.sa_flags = 0;
    sigemptyset(&ignore_action.sa_mask);
    if (sigaction(SIGINT, &ignore_action, &save_interrupt_action) < 0) {
        return -1;
    }
    if (sigaction(SIGQUIT, &ignore_action, &save_interrupt_action) < 0) {
        return -1;
    }

    //2. 屏蔽掉子进程信号
    sigemptyset(&child_mask);
    sigaddset(&child_mask, SIGCHLD);
    if (sigprocmask(SIG_BLOCK, &child_mask, &save_mask)) {
        return -1;
    }

    //3. fork
    if ((pid = fork()) < 0) {
        status = -1;
    } else if (pid == 0) {
        //4. 子进程中恢复屏蔽字，恢复处理动作
        sigaction(SIGINT, &save_interrupt_action, NULL);
        sigaction(SIGQUIT, &save_interrupt_action, NULL);
        sigprocmask(SIG_SETMASK, &save_mask, NULL);

        //5. 子进程中执行exec，exit
        execl("/bin/sh", "sh", "-c", cmd_string, (char *)0);
        _exit(127);
    } else {
        //6. 父进程等待子进程
        while (waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1; /* error other than EINTR from waitpid() */
                break;
            }
        }
    }

    if (sigaction(SIGINT, &save_interrupt_action, NULL) < 0) {
        return -1;
    };
    if (sigaction(SIGQUIT, &save_interrupt_action, NULL) < 0) {
        return -1;
    }
    if (sigprocmask(SIG_SETMASK, &save_mask, NULL) < 0) {
        return -1;
    }

    return status;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        err_sys("argc error");
    }

    char cmd_string[128];
    strcpy(cmd_string, argv[1]);
    for (int i = 2; i < argc; ++i) {
        strcat(cmd_string, " ");
        strcat(cmd_string, argv[i]);
    }
    puts(cmd_string);

    return my_system(argv[1]);
}
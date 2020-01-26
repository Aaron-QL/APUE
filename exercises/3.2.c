#include "../apue.3e/include/apue.h"

struct Node {
    int fd;
    struct Node *next;
};

void free_fd(struct Node *head)
{
    struct Node *temp;
    while (head->next != NULL) {
        close(head->next->fd);
        printf("close fd %d\n", head->next->fd);
        temp = head->next;
        free(head);
        head = temp;
    }
}

int my_dup2(int fd1, int fd2)
{
    if (fd1 == fd2) {
        return fd2;
    }

    close(fd2);

    struct Node *head = (struct Node *) malloc(sizeof(struct Node));
    struct Node *cur = head;
    struct Node *temp;
    int fd_temp;
    while ((fd_temp = dup(fd1)) != fd2) {
        printf("create fd %d\n", fd_temp);
        if (fd_temp == -1) {
            free_fd(head);
            return -1;
        }
        temp = (struct Node *) malloc(sizeof(struct Node));
        temp->fd = fd_temp;
        temp->next = NULL;
        cur->next = temp;
        cur = cur->next;
    }

    free_fd(head);

    return fd2;
}

int main()
{
    int fd = my_dup2(STDOUT_FILENO, 15);
    write(fd, "hello, world\n", 13);
    return 0;
}
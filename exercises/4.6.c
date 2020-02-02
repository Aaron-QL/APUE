#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFSIZE 4096

//复制一个带空洞的文件
int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("Usage: ./a.out file1 file2\n");
        exit(1);
    }

    int fd1, fd2;

    if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
        printf("open %s failed", argv[1]);
        exit(1);
    }

    if ((fd2 = open(argv[2], O_RDWR | O_CREAT, 0666)) < 0) {
        printf("open %s failed", argv[2]);
        exit(1);
    }

    int cnt, flag, l;
    char buff[BUFFSIZE];
    while ((cnt = read(fd1, buff, BUFFSIZE))) {
        flag = 0;
        l = 0;
        for (int i = 0; i < cnt; i++) {
            if (flag == 0 && buff[i] != 0) {//从空洞遇到非空洞，从当前位置位移l长度，l重置为1，flag置为1
                if (lseek(fd2, l, SEEK_CUR) < 0) {
                    printf("lseek failed");
                    exit(1);
                }
                l = 1;
                flag = 1;
            } else if (flag == 1 && buff[i] == 0) {//从非空洞遇到空洞，从i - l的位置写入l长度，l重置为1，flag置为0
                if (write(fd2, buff + i - l, l) != l) {
                    printf("write failed");
                    exit(1);
                }
                l = 1;
                flag = 0;
            } else {//其他情况，l++
                l++;
            }
        }
        //将剩下的部分写入或位移
        if (flag) {
            if (write(fd2, buff + cnt - l, l) != l) {
                printf("write failed");
                exit(1);
            }
        } else {
            if (lseek(fd2, l, SEEK_CUR) < 0) {
                printf("lseek failed");
                exit(1);
            }
        }
    }

    return 0;
}
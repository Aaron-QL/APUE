#include "../apue.3e/include/apue.h"
#include <fcntl.h>

// 声明两个字符串
char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
    int fd;
    // 创建文件
    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        err_sys("create error");
    }

// 写入第一个字符串
    if (write(fd, buf1, 10) != 10) {
        err_sys("buf1 write error");
    }

 // lseek
    if (lseek(fd, 16384, SEEK_SET) == -1) {
        err_sys("lseek error");
    }


// 写入第二个字符串
    if (write(fd, buf2, 10) != 10) {
        err_sys("buf2 write error");
    }

    exit(0);
}




// 退出


//int main(void)
//{
//    int fd;
//
//    fd = open("file.hole", O_RDWR | O_TRUNC);
//
//    int i = 0;
//    while (i++ < 1000) {
//        if (lseek(fd, 3, SEEK_CUR) == -1) {
//            err_sys("lseek error");
//        }
//
//        if (write(fd, "abc", 3) != 3) {
//            err_sys("buf2 write error");
//        }
//
//    }
//
//    exit(0);
//}
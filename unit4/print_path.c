#include "../apue.3e/include/apue.h"
#include <dirent.h>
#include <limits.h>

#define MAX_PATH_LEN 256
static char path_buffer[MAX_PATH_LEN];

//遍历目录树

static void travel_dir(char *path, int deep)
{
    DIR *dirptr;
    struct dirent *entity;
    struct stat file_status;

    //获取文件状态，并检测是否是目录
    if (stat(path, &file_status) < 0 || !S_ISDIR(file_status.st_mode)) {
//        printf("invalid path: %s\n", path);
        return;
    }

    //打开目录
    if ((dirptr = opendir(path)) == NULL) {
        printf("opendir[%s] error: %m\n", path);
        return;
    }

    //遍历目录
    while ((entity = readdir(dirptr)) != NULL) {
        //过滤掉. 和 .. 目录项
        if (strcmp(entity->d_name, ".") == 0 || strcmp(entity->d_name, "..") == 0) {
            continue;
        }
        //拼接目录项全路径
        snprintf(path_buffer, MAX_PATH_LEN, "%s/%s", path, entity->d_name);

        //获取目录项文件状态
        if (stat(path_buffer, &file_status) < 0) {
//            printf("invalid path: %s\n", path_buffer);
            continue;
        }

        //打印文件类型
//        switch (file_status.st_mode & S_IFMT) {
//            case S_IFREG:
//                printf("regular   ");
//                break;
//            case S_IFBLK:
//                printf("block     ");
//                break;
//            case S_IFCHR:
//                printf("character ");
//                break;
//            case S_IFIFO:
//                printf("fifo      ");
//                break;
//            case S_IFLNK:
//                printf("link      ");
//                break;
//            case S_IFSOCK:
//                printf("socket    ");
//                break;
//            case S_IFDIR:
//                printf("directory ");
//                break;
//            default:
//                break;
//        }
        printf("|");
        for (int i = 0; i < deep; ++i) {
            printf("--");
        }

        //打印目录项名称
        printf("%s\n", entity->d_name);

        if (S_ISDIR(file_status.st_mode)) {
            travel_dir(path_buffer, deep + 1);
        }
    }
    closedir(dirptr);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s [dir]\n", argv[0]);
        exit(1);
    }

    travel_dir(argv[1], 0);
    exit(0);
}
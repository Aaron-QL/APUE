#include "../apue.3e/include/apue.h"
#include <dirent.h>
#include <limits.h>

/**
1.main
接受参数
调用myftw
打印各项数据
退出

2.myftw
为路径变量分配空间并初始化
 调用dopath

3.dopath
获取文件结构
 判断类型
 如果不是文件夹、直接调用myfunc
 如果是文件夹，先调用一次myfunc
 然后更新路径变量，打开文件夹
    为每个子文件调用myfunc
    关闭文件夹，更新文件路径
 返回

4.myfunc
判断类型，增加相应变量
**/
typedef int Myfunc(const char *, struct stat *, int);
static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int main(int argc, char **argv) {
    if (argc != 2) {
        err_quit("usage:  ftw  <starting-pathname>");
    }

    int ret = myftw(argv[1], myfunc);

    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0) {
        ntot = 1;
    }

    printf("regular files  = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories    = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special  = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special   = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFOs          = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets        = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);

    exit(ret);
}

static char *fullpath;
static size_t pathlen;

static int myftw(char *pathname, Myfunc *myfunc)
{
    fullpath = path_alloc(&pathlen);
    if (pathlen < strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
            err_sys("realloc failed");
        }
    }
    strcpy(fullpath, pathname);
    return dopath(myfunc);
}

#define	FTW_F	1		/* file other than directory */
#define	FTW_D	2		/* directory */
#define	FTW_DNR	3		/* directory that can't be read */
#define	FTW_NS	4		/* file that we can't stat */

static int dopath(Myfunc *func)
{
    struct stat statbuf;
    if (lstat(fullpath, &statbuf) < 0) {
        return func(fullpath, &statbuf, FTW_NS);
    }
    if (S_ISDIR(statbuf.st_mode) == 0) {
        return func(fullpath, &statbuf, FTW_F);
    }

    int ret, n;
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0) {
        return ret;
    }

    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen) {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
            err_sys("realloc failed");
        }
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;

    DIR *dirp;
    if ((dirp = opendir(fullpath)) == NULL) {
        return func(fullpath, &statbuf, FTW_DNR);
    }

    struct dirent *direntp;
    while ((direntp = readdir(dirp)) != NULL) {
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0) {
            continue;
        }
        strcpy(&fullpath[n], direntp->d_name);
        if ((ret = dopath(func)) != 0) {
            break;
        }
    }
    fullpath[n - 1] = 0;

    if (closedir(dirp) < 0) {
        err_ret("can't close directory %s", fullpath);
    }

    return ret;
}

static int myfunc(const char *pathname, struct stat *statptr, int type)
{
    switch (type) {
        case FTW_F:
            switch (statptr->st_mode & S_IFMT) {
                case S_IFREG: nreg++;       break;
                case S_IFBLK: nblk++;       break;
                case S_IFCHR: nchr++;       break;
                case S_IFIFO: nfifo++;      break;
                case S_IFLNK: nslink++;     break;
                case S_IFSOCK: nsock++;     break;
                case S_IFDIR:
                    err_dump("for S_IFDIR for %s", pathname);
            }
            break;
        case FTW_D:
            ndir++;
            break;
        case FTW_DNR:
            err_ret("can't read directory %s", pathname);
            break;
        case FTW_NS:
            err_ret("stat error for %s", pathname);
            break;
        default:
            err_dump("unknown type %d for pathname %s", type, pathname);
    }
    return 0;
}
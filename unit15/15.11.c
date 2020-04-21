#include "../apue.3e/include/apue.h"
#include <sys/wait.h>

#define	PAGER	"${PAGER:-more}" /* environment variable, or default */

int main(int argc, char *argv[])
{
    FILE *fp_in, *fp_out;
    char line[MAXLINE];

    if (argc != 2) {
        err_sys("error");
    }

    if ((fp_in = fopen(argv[1], "r")) == NULL) {
        err_sys("error");
    }

    if ((fp_out = popen(PAGER, "w")) == NULL) {
        err_sys("error");
    }

    while (fgets(line, MAXLINE, fp_in) != NULL) {
        if (fputs(line, fp_out) == EOF) {
            err_sys("error");
        }
    }
    if (ferror(fp_in)) {
        err_sys("error");
    }
    if (pclose(fp_out) < 0) {
        err_sys("error");
    }
    exit(0);
}
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int fds[2];
    pipe(fds);
    int buf[3000] = {0};
    int pid = fork();

    if (pid > 0) {
        for (int i = 2; i <= 280; i++) {
            // write(fds[1], i, 4);
        }
    } else if (pid == 0) {

    } else {
        printf("Fork error\n");
    }

    free(buf);
    exit(0);
}
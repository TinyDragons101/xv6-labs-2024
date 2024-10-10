#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p[2];
    pipe(p); // create pipe for p

    int pid = fork();

    if (pid > 0) {
        // parent
        int pid_parent = getpid();
        
        close(p[0]);

        char send_msg = 's';
        write(p[1], &send_msg, 1); // write a byte to file descriptor referring the write end of a pipe;
        close(p[1]);
        wait(0);

        char msg;
        if(read(p[0], &msg, 1))
            printf("%d: received pong\n", pid_parent);

        close(p[0]);
        exit(0);
    }
    else if (pid == 0) {
        
        int pid_child = getpid();
        // child read
        close(p[1]);
        char msg;
        if(read(p[0], &msg, 1))
            printf("%d: received ping\n", pid_child);

        close(p[0]); // close read end after reading;

        // child write
        write(p[1], &msg, 1); // write 1 byte to the parent;
        close(p[1]); // close the write end of the pipe;
        exit(0); // exit the child;
    } else {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    exit(0);
}
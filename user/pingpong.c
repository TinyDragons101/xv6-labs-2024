#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int fds1[2]; // file descriptors
    pipe(fds1); // create pipe from parent to child

    int fds2[2];
    pipe(fds2); // create pipe from child to parent

    int pid = fork();

    if (pid > 0) {
        // parent
        int pid_parent = getpid();
        
        char parent_msg = 's';
        write(fds1[1], &parent_msg, 1); 

        close(fds1[1]);
        wait(0);

        if(read(fds2[0], &parent_msg, 1))
            printf("%d: received pong\n", pid_parent);

        close(fds2[0]);
        exit(0);
    }
    else if (pid == 0) {
        // child
        int pid_child = getpid();

        char child_msg;
        if(read(fds1[0], &child_msg, 1))
            printf("%d: received ping\n", pid_child);

        close(fds1[0]); 

        write(fds2[1], &child_msg, 1); // write 1 byte to the parent;
        
        close(fds2[1]); // close the write end of the pipe;
        exit(0); // exit the child;
    } else {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    exit(0);
}
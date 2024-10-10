#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int fds[2]; // file descriptors
    pipe(fds); // create pipe for fds

    int pid = fork();

    if (pid > 0) {
        // parent
        int pid_parent = getpid();
        
        char parent_msg = 's';
        write(fds[1], &parent_msg, 1); 

        close(fds[1]);
        wait(0);

        if(read(fds[0], &parent_msg, 1))
            printf("%d: received pong\n", pid_parent);

        close(fds[0]);
        exit(0);
    }
    else if (pid == 0) {
        // child
        int pid_child = getpid();

        char child_msg;
        if(read(fds[0], &child_msg, 1))
            printf("%d: received ping\n", pid_child);

        close(fds[0]); 

        write(fds[1], &child_msg, 1); // write 1 byte to the parent;
        
        close(fds[1]); // close the write end of the pipe;
        exit(0); // exit the child;
    } else {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    exit(0);
}
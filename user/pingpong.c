#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

<<<<<<< HEAD
int main() {
    int fds1[2]; // file descriptors
    pipe(fds1); // create pipe from parent to child

    int fds2[2];
    pipe(fds2); // create pipe from child to parent

    int pid = fork();
=======
int main(int argc, char *argv[]) {

    // argument handling
    if (argc != 1) {
        fprintf(2, "pingpong: no arguments needed\n");
        exit(1);
    }

    int fds_p[2]; // file descriptors
    pipe(fds_p); // create pipe from parent to child

    int fds_c[2];
    pipe(fds_c); // create pipe from child to parent

    int pid = fork();   
>>>>>>> remote-repo2/main

    if (pid > 0) {
        // parent
        int pid_parent = getpid();
        
        char parent_msg = 's';
<<<<<<< HEAD
        write(fds1[1], &parent_msg, 1); 

        close(fds1[1]);
        wait(0);

        if(read(fds2[0], &parent_msg, 1))
            printf("%d: received pong\n", pid_parent);

        close(fds2[0]);
=======
        write(fds_p[1], &parent_msg, 1); 

        close(fds_p[1]);
        wait(0);

        if(read(fds_c[0], &parent_msg, 1)){
            printf("%d: received pong\n", pid_parent);
        }

        close(fds_c[0]);
>>>>>>> remote-repo2/main
        exit(0);
    }
    else if (pid == 0) {
        // child
        int pid_child = getpid();

        char child_msg;
<<<<<<< HEAD
        if(read(fds1[0], &child_msg, 1))
            printf("%d: received ping\n", pid_child);

        close(fds1[0]); 

        write(fds2[1], &child_msg, 1); // write 1 byte to the parent;
        
        close(fds2[1]); // close the write end of the pipe;
=======
        if(read(fds_p[0], &child_msg, 1)){
            printf("%d: received ping\n", pid_child);
        }
        close(fds_p[0]); 

        write(fds_c[1], &child_msg, 1); // write 1 byte to the parent;
        
        close(fds_c[1]); // close the write end of the pipe;
>>>>>>> remote-repo2/main
        exit(0); // exit the child;
    } else {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    exit(0);
<<<<<<< HEAD
}
=======
}
// Tan's code
>>>>>>> remote-repo2/main

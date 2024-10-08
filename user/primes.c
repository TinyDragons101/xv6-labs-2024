#include "kernel/types.h"
#include "user/user.h"

void sieve(int left_p[2]) __attribute__((noreturn)); // the function doesn't return
void sieve(int left_p[2]) {
    // close write end of left pipe
    close(left_p[1]);
    // read prime from the left pipe
    int p;
    if (read(left_p[0], &p, sizeof(int)) == 0) {
        // no read, close the process
        close(left_p[0]);
        // do i have to wait
        exit(0);
    }

    // print the prime
    printf("prime %d\n", p);

    // create pipe to push the sieved product to the child processes 
    int right_p[2];
    pipe(right_p);
    
    int fid = fork();

    if (fid > 0) {
        // read from left pipe, if it is not divisible by p then push to the right pipe
        close(right_p[0]);

        int n;
        while(read(left_p[0], &n, sizeof(int)) > 0) {
            if (n % p != 0) {
                write(right_p[1], &n, sizeof(int));
            }
        }

        // close write and read ends in while loop
        close(right_p[1]); 
        close(left_p[0]);

        wait(0);
        exit(0);
    }

    else {
        // if this is the child then process the right_p
        close(left_p[0]); // close read of left pipe before continuing
        sieve(right_p);
    }

    // close read port
    close(right_p[0]);


}

int
main() {
    // create a pipe
    int p[2];
    pipe(p);

    int fid = fork();
    if (fid > 0) {
        // parent
        // feed the numbers from 2 to 280 to the pipe for the child to sieve
        // close read end
        close(p[0]);
        for(int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(int));
        }

        // close write end
        close(p[1]);
        // wait for all child to exit
        wait(0);
    }

    else {
        // child
        // close write end 
        close(p[1]);
        sieve(p);

        // // close read and write end
        close(p[0]);
        close(p[1]);
        // wait for all child to exit
        wait(0);
    }

    exit(0);
}
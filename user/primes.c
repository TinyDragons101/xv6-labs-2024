#include "kernel/types.h"
#include "user/user.h"

void sieve(int left_p[2]) {
    close(left_p[1]);  // close write end of left pipe
    
    int p;
    if (read(left_p[0], &p, sizeof(int)) == 0) {
        close(left_p[0]);
        exit(0);
    }

    printf("prime %d\n", p);

    int n;
    int buf[32];  // Buffer to store numbers before forwarding them
    int count = 0;
    
    // Collect numbers that are not divisible by p
    while (read(left_p[0], &n, sizeof(int)) > 0) {
        if (n % p != 0) {
            buf[count++] = n;
            if (count == 32) {
                break;  // Process 32 numbers at a time
            }
        }
    }
    close(left_p[0]);  // close read end

    // If there are more numbers, pass them to the next sieve stage
    if (count > 0) {
        int right_p[2];
        pipe(right_p);

        if (fork() > 0) {
            close(right_p[0]);  // parent only writes to right pipe

            for (int i = 0; i < count; i++) {
                write(right_p[1], &buf[i], sizeof(int));
            }

            while (read(left_p[0], &n, sizeof(int)) > 0) {
                if (n % p != 0) {
                    write(right_p[1], &n, sizeof(int));
                }
            }

            close(right_p[1]);  // close write end
            wait(0);            // wait for child to finish
            exit(0);
        } else {
            close(right_p[1]);  // child only reads from right pipe
            sieve(right_p);
        }
    }
}

int main(int argc, char* argv[]) {
    // argument checking
    if (argc != 1) {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    int p[2];
    pipe(p);

    if (fork() > 0) {
        // parent: feed numbers from 2 to 283 to the pipe
        for (int i = 2; i <= 283; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);  // close write end
        wait(0);      // wait for child to finish
    } else {
        sieve(p);     // child runs the sieve
    }

    exit(0);
}

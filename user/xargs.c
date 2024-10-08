#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int
main(int argc, char* argv[]) {

    // error handling
    if (argc < 2) {
        fprintf(2, "xargs: no command given\n");
        exit(1);
    }
    
    char* cmd = argv[1];
    char* args[MAXARG];
    int i = 0;

    // copy the arguments to a new array
    // the last argument is the input: echo "hello" | xargs echo bye -> xargs echo bye "hello" 
    for(i = 1; i < argc; i++) {
        args[i-1] = argv[i];
    }
    args[argc-1] = 0; // reset the last argument to 0

    // read from stdin to c
    char c;

    // buffer
    char buf[512];
    // count number of characters in the buffer
    int cnt = 0;
    while (read(0, &c, sizeof(char)) != 0 ) {
        // parent
        // split the input by '\n'
        // create a child process for each line
        if (c == '\n') {
            buf[cnt] = '\0';
            int pid = fork();
            if (pid == 0) {
                // child

                // set the last argument to the read line
                args[argc-1] = buf;
                exec(cmd, args);
                exit(0);
            }
            else {
                // parent

                // reset the buffer
                cnt = 0;
                memset(buf, 0, sizeof(buf));

                // wait for the child to finish
                wait(0);
            }
        }
        else {
            // add c to buffer if c is not '\n'
            buf[cnt++] = c;
        }
    }
}
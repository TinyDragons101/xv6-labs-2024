#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) {

    if (argc == 1) {
        // 0 stdin, 1 stdout, 2 stderr
        fprintf(2, "User forget to pass an argument.\n");
        exit(1);
    }

    if (argc > 2) {
        fprintf(2, "User pass too many arguments.\n");
        exit(1);
    }

    int tick = atoi(argv[1]);
    if (tick < 0) tick = 0;

    // argv[1] is string of time
    
    fprintf(1, "(nothing happens for a little while)\n");
    sleep(tick);    // sleep for some ticks
    exit(0);
}
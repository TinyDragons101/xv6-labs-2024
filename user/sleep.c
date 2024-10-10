#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        // 0 stdin, 1 stdout, 2 stderr
        fprintf(2, "Error! Missing arguments");
        exit(1);
    }

    // argv[1] is string of time
    int ticks = atoi(argv[1]);
    if (ticks < 0) ticks = 0;

    fprintf(1, "(nothing happens for a little while)\n");
    sleep(ticks);   // sleep for some ticks
    exit(0);
}
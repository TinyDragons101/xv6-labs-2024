#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/syscall.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "usage: %s tracemask prog ...\n", argv[0]);
        exit(1);
    }

    int tracemask = atoi(argv[1]);
    trace(tracemask);

    char **newargv = argv + 2;
    exec(argv[2], newargv);
}

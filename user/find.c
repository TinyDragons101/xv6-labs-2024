#include "user/user.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void
find(char *path, char *fileName)
{
    int fds;
    char buf[512];
    char *p;
    struct dirent de;
    struct stat st;

    if((fds = open(path, O_RDONLY)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }
    

    if((fstat(fds, &st)) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fds);
        return;
    }

    switch(st.type){
        case T_DEVICE:
        case T_FILE:
            if(strcmp(fmtname(path), fileName) == 0){
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';

            while(read(fds, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0){
                    continue;
                }

                memmove(p, de.name, strlen(de.name));
                p[strlen(de.name)] = 0;

                if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
                    continue;
                }

                find(buf, fileName);
            }
            break;
    }
    close(fds);
}

int
main(int argc, char *argv[])
{
    if (argc < 3){
        fprintf(2, "Error! Missing arguments");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}
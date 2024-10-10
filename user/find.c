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
    char buf[50];
    char *p;
    struct dirent de;
    struct stat st;

    // Read stat
    if(stat(path, &st) != 0){
        return;
    }
    
    // Eliminate recursion of . and ..
    if((strcmp(fmtname(path), ".") == 0 && strlen(path) > 1) || strcmp(fmtname(path), "..") == 0){
        return;
    }

    switch(st.type){
        case T_DEVICE:
        case T_FILE:
            if(strcmp(fmtname(path), fileName) == 0){
                fprintf(2, "%s\n", path);
                return;
            }
            break;
        case T_DIR:
            // if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
            //     break;
            // }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';

            int fd = open(path, O_RDONLY); 
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, strlen(de.name));
                p[strlen(de.name)] = 0;
                find(buf, fileName);
            }
            break;
    }
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
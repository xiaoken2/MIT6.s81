#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

char* getName(char* path) {
    char* p;
    for (p = path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    return p;
}

// void find(char* path, char* str) {
//     char buf[512]; //为什么是512？
//     struct dirent de;
//     struct stat st; //这里是什么意思？

//     int fd = open(path, 0);

//     if (fd < 0) {
//         fprintf(2, "find: cannot open %s\n", path); 
//         return;
//     }
//     if (fstat(fd, &st) < 0) {
//         fprintf(2, "find: cannot stat %s\n", path); 
//         close(fd);
//         return;
//     }

//     switch (st.type) {
//         case T_FILE: // 处理普通文件
//             if (!strcmp(str, getName(path))){
//                 printf("%s\n", path);
//             }
//             break;

//         case T_DIR: //处理目录
//             strcpy(buf, path);
//             char* p = buf + strlen(buf);
//             *p = '/';
//             while (read(fd, &de, sizeof(de)) == sizeof(de)) {
//                 if (de.inum == 0) {
//                     continue;
//                 }
//                 memmove(p, de.name, DIRSIZ);
//                 p[DIRSIZ] = 0;
//                 if (stat(buf, &st) < 0) {
//                     printf("find: connot stat %s\n", buf);
//                     continue;
//                 }

//                 if (st.type == T_FILE || st.type == T_DEVICE) {
//                     if (!strcmp(str, getName(buf))) {
//                         printf("%s\n", buf);
//                     } 
//                 } else if (st.type == T_DIR && strcmp(".", getName(buf)) && strcmp("..", getName(buf))) {
//                         find(buf, str);
//                     }
//             }
//         break;
//     }
//     close(fd);

// }
void find(char* path, char* str) {
    char buf[512];
    struct dirent de;
    struct stat st;

    int fd = open(path, 0);
    if (fd < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_FILE:
        if (!strcmp(str, getName(path))) {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        strcpy(buf, path);
        char* p = buf + strlen(buf);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
                continue;

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if (stat(buf, &st) < 0) {
                printf("find: cannot stat %s\n", buf);
                continue;
            }

            if (st.type == T_FILE) {
                if (!strcmp(str, getName(buf))) {
                    printf("%s\n", buf);
                }
            } else if (st.type == T_DIR) {
                find(buf, str);
            }
        }
        break;
    }

    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc == 3) find(argv[1], argv[2]);
    else printf("argument error\n");

    exit(0);
}
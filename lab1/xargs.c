#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXARG 32  // 定义最大参数数量

int main(int argc, char *argv[]) {
    char *p[MAXARG];
    int i;

    // 将命令行参数复制到 p 数组中
    for (i = 1; i < argc; i++) {
        p[i - 1] = argv[i];
    }

    // 为最后一个参数分配内存，并设置参数数组的结束符
    p[argc - 1] = malloc(512);
    p[argc] = 0;

    // 逐行读取输入，并执行命令
    while (gets(p[argc - 1], 512)) {
        if (p[argc - 1][0] == 0) break;  // 已读完

        // 去掉换行符
        if (p[argc - 1][strlen(p[argc - 1]) - 1] == '\n') {
            p[argc - 1][strlen(p[argc - 1]) - 1] = 0;
        }

        // 创建子进程并执行命令
        if (fork() == 0) {
            exec(argv[1], p);
            printf("exec %s failed\n", argv[1]);
            exit(1);
        } else {
            wait(0);  // 在父进程中等待子进程完成
        }
    }

    exit(0);
}



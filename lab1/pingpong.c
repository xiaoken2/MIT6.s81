#include "kernel/types.h"
#include "user/user.h"

int main() {
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    char c[] = {'x'};
    int pid = fork();

    if (pid == 0) {
        close(p1[1]);
        close(p2[0]);

        read(p1[0], &c ,1);
        printf("received ping\n");
        write(p2[1], &c, 1);

    } else if (pid > 0) {
        close(p1[0]);
        close(p2[1]);

        write(p1[1], &c,1);
        read(p2[0], &c, 1);

        printf("received pong\n");
    } else {
        printf("not fork\n");
    }

    exit(0);

}
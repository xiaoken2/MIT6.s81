#include "kernel/types.h"
#include "user/user.h"

void getPrimes(int readFok) {
    int p;
    if (read(readFok, &p, sizeof(p)) == 0) {
        close(readFok);
        return;
    }

    printf("primes %d\n", p);
    int p1[2];
    pipe(p1);

    if (fork() == 0) {
        close(p1[1]);
        getPrimes(p1[0]);
    } else {
        int n;
        close(p1[0]);
        while (read(readFok, &n, sizeof(n)) > 0) {
            if (n % p) {
                write(p1[1], &n, sizeof(n));
            }
        }
        close(p1[1]);
        wait(0);
        exit(0);
    }
}

int main() {
    int fok[2];
    pipe(fok);

    if (fork() == 0) {
        close(fok[1]);
        getPrimes(fok[0]);
    } else {
        close(fok[0]);
        for (int i = 2; i <= 35; i++) {
            write(fok[1], &i, sizeof(i));
        }
        close(fok[1]);
        wait(0);
    }

    exit(0);
}
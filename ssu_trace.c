#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int pid;
    if(argc < 3) {
        printf(1, "error: ssu_trace [mask] [command]\n");
        exit();
    }

    pid = fork();
    if(pid < 0) {
        printf(1, "ssu_trace: fork failed\n");
    }
    if (pid == 0) {
        trace(atoi(argv[1]));
        exec(argv[2], argv+2);
    } else if (pid > 0) {
        wait();
    }
    exit();
}
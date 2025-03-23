#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid: %d)\n", getpid());
    int rc = fork();
    if (rc < 0) { // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (new process)
        printf("hello, I am child (pid: %d)\n", getpid());
    } else { // parent goes down this path (main)
        //int wc = wait();
        printf("hello, I am parent of %d (pid: %d)\n", rc, getpid());
    }
    return 0;
}

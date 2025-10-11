#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
     printf("Parent started (pid:%d)\n", getpid());
    int rc = fork();
    if (rc < 0) {
        printf("fork failed\n");
        exit(1);
    } else if (rc == 0) { // child

        printf("Child (pid:%d)\n", getpid());
        char *argv[] = {"wc", "text1.txt", NULL};
        execvp("wc", argv);
        //if execvp is successful, you don't get here

    } else { // parent
        printf("Parent (pid:%d)\n", getpid());
    }
}

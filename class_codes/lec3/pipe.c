#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    int rc = fork();
    if (rc < 0) {
        perror("fork");
        exit(1);
    } else if (rc == 0) {
        // CHILD PROCESS
        close(pipefd[0]); // close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe
        close(pipefd[1]); // safe to close after dup2

        char *myargs[3];
        myargs[0] = strdup("wc");
        myargs[1] = strdup("text1.txt");
        myargs[2] = NULL;

        execvp(myargs[0], myargs);

        //these line should not execute
        //if exec was successful
        perror("execvp failed");
        exit(1);
    } else {
        // PARENT PROCESS
        close(pipefd[1]); // close unused write end
        wait(NULL); // wait for child to finish

        char buffer[128];
        int n = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            int lines, words, chars;
            sscanf(buffer, "%d %d %d", &lines, &words, &chars);

            printf("Data received from the child process indicates there are %d new line characters, %d characters, and %d words in the file.\n",
                   lines, chars, words);
        } else {
            printf("No data received from child.\n");
        }

        close(pipefd[0]);
        printf("Parent done.\n");
    }

    return 0;
}

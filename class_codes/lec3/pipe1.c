#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(void){

    int fd[2];
    int rc = pipe(fd);
    int FD_PREAD, FD_PWRITE;

    if(rc < 0){
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }else{
        FD_PREAD = fd[0];
        FD_PWRITE = fd[1];
    }

    rc = fork();

    if(rc<0){
        perror("fork failed");
        exit(EXIT_FAILURE);
    }else if(rc ==0){
        //child
        close(FD_PREAD);
        dup2(FD_PWRITE, STDOUT_FILENO); //error check should be added
                                        //do man dup2

        write(STDOUT_FILENO, "Data from the child...", 23);
    }else{
        //parent 
        close(FD_PWRITE);
        dup2(FD_PREAD, STDIN_FILENO); //error check should be added
                                      //do man dup2

        rc = wait(NULL); //wait for the child to finish, reap
        char input[256];
        read(STDIN_FILENO, input, 23);
        input[23] = '\0';

        printf("From child: %s\n", input);
    }

    return 0;
}

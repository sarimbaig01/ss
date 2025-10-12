#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void){
    char input[20];
    int fd = open("file1.txt", O_RDONLY);
    int chars_read = read(fd, input, 15);
    write(STDOUT_FILENO,input,chars_read);
}

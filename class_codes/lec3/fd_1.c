#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    char input[20];
    int chars_read = read(STDIN_FILENO, input, 15);
    write(STDOUT_FILENO,input,chars_read);
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    int * p = malloc(sizeof(int));

    *p = (int)argv[1][0]; //first character only

    printf("Process: %s, Address: %p, Value: %d\n", argv[1], (void*)p, *p);

    free(p);

    return 0;
}

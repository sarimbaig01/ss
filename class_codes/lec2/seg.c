#include <stdio.h>
#include <stdlib.h>

int global_ui1, global_ui2;

int global_i1 = 50, global_i2 = 30;

int global_zi = 0;

int main(){

    int local1, local2;

    int * p = (int*)malloc(100);

    printf(".text address:%p\n", (void*)main);

    printf(".stack addresses:%p, %p, ...\n", &local1, &local2);

    printf(".heap addresses:%p, ...\n", p);

    printf(".data addresses:%p, %p, ...\n",  &global_i1, &global_i2);

    printf(".bss addresses:%p, %p, %p...\n",  &global_ui1, &global_ui2, &global_zi);
}

#include <stdio.h>
#include <unistd.h> 

int main(int argc, char *argv[]) {

    char *msg = argv[1];

    while (1) {
        printf("%s\n", msg);
        sleep(1); // wait 1 second
    }

    return 0;
}

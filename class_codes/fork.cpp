#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    std::cout << "hello world (pid:" << getpid() << ")\n";

    int rc = fork();
    if (rc < 0) { // fork failed; exit
        std::cerr << "fork failed\n";
        exit(1);
    } else if (rc == 0) { // child (new process)
        std::cout << "hello, I am child (pid:" << getpid() << ")\n";
    } else { // parent goes down this path (main)
        std::cout << "hello, I am parent of " << rc
                  << "(pid:" << getpid() << ")\n";
    }
    return 0;
}

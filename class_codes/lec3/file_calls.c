#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

int main() {
    char buffer[128];
    int fd = open("file.txt", O_RDONLY);  // Open file for reading
    if (fd == -1) {
        perror("open");
        return 1;
    }

    ssize_t bytes_read;
    int vowel_count = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            char c = tolower(buffer[i]);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            }
        }
    }

    if (bytes_read == -1) {
        perror("read");
    }

    close(fd);
    printf("Total vowels: %d\n", vowel_count);
    return 0;
}

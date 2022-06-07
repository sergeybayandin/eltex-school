#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FILENAME ".file"

int main(void)
{
    int fd;

    if ((fd = open(FILENAME, O_CREAT | O_RDWR)) < 0) {
        perror("Failed to creat file");
        return -1;
    }

    const char *str = "eltex-school";
    const int len = strlen(str);

    if (write(fd, str, len) < 0) {
        perror("Failed to write file");
        return -1;
    }

    int i;
    char buf[16];

    memset(buf, 0, sizeof(buf));

    for (i = 0; i < len; ++i) {
        if (lseek(fd, len - i - 1, SEEK_SET) == (off_t)-1 || read(fd, buf + len - i - 1, 1) < 0) {
            perror("Failed to read file");
            return -1;
        }
    }

    printf("Message: %s\n", buf);

    if (close(fd) != 0 || remove(FILENAME) != 0) {
        perror("Failed to end file");
        return -1;
    }

    return 0;
}

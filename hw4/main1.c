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

    ssize_t count;
    const char *str = "eltex-school";

    if ((count = write(fd, str, strlen(str))) < 0) {
        perror("Failed to write file");
        return -1;
    }

    printf("Written: %d\n", (int)count);

    char str_buf[16];

    memset(str_buf, 0, sizeof(str_buf));

    if (lseek(fd, 0, SEEK_SET) == (off_t)-1 || (count = read(fd, str_buf, sizeof(str_buf))) < 0) {
        perror("Failed to read file");
        return -1;
    }

    printf("Read: %d\n", (int)count);
    printf("Message: %s\n", str_buf);

    if (close(fd) != 0 || remove(FILENAME) != 0) {
        perror("Failed to end file");
        return -1;
    }

    return 0;
}

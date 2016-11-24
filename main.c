#include "code.h"

int main(int argc, char **argv)
{
    int fd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    fd = open(argv[1], O_RDONLY);
    computer(fd);
    close(fd);

    return 0;
}

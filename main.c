#include "code.h"

int main(int argc, char **argv)
{
    int fd;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    } else if (argc < 2) {
        fd = dup(STDIN_FILENO);
    } else {
        fd = open(argv[1], O_RDONLY);
    }
    calculator(fd);
    close(fd);

    return 0;
}

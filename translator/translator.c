#include "../code.h"

static int translator(const char*, FILE*);
int main(int argc, char **argv)
{
    int opt;
    FILE *fp;

    while ((opt = getopt(argc, argv, "o:")) != -1) {
        if (opt == 'o') {
            fp = fopen(optarg, "w");
        } else { /* '?' */
            fprintf(stderr, "Usage: %s infile [-o outfile]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (translator(argv[optind], fp) != 0) {
        goto end;
    }

end:
    fclose(fp);
    return 0;
}

static void inttobool8(int a, bool b[])
{
}

static void inttobool16(int a, bool b[])
{
    for (int i = 15; i >= 0; --i) {
        b[i] = a & 0x01;
        a >>= 1;
    }
}

static int translator(const char *infile, FILE *outfile)
{
    FILE *infp;
    char buf[BUFSIZ];
    unsigned short addr;
    bool addrbuf[16];
    bool insbuf[8];

    printf("%s\n", infile);
    if((infp = fopen(infile, "r")) == NULL) {
        fprintf(stderr, "Translate error: %s\n", strerror(errno));
        return -1;
    }

    addr = 0;
    while (fgets(buf, BUFSIZ, infp) != NULL) {
        inttobool16(addr, addrbuf);
        addr++;
    }

    fclose(infp);
    
    return 0;
}

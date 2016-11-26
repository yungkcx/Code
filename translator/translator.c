#include "../code.h"

#define SAVE        0xC0
#define COMMENT     0xC1
#define BLANK_LINE  0xC2

#define MAXLINE 1024
#define STRNCASECMP(a, R, b, n) (strncasecmp(a, b, n) R 0)

static int error_line = 0;

typedef struct {
    char *line;
} trans_line;

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
    if (optind != argc - 1) {
        fprintf(stderr, "Usage: %s infile [-o outfile]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argc == 2)
        fp = fopen("a.b", "w");

    errno = 0;
    if (translator(argv[optind], fp) != 0) {
        fprintf(stderr, "Translate error: %d: %s\n", error_line,
                errno ? strerror(errno) : "syntax error");
        goto end;
    }

end:
    fclose(fp);
    return 0;
}

static void inttobool8(int a, bool b[])
{
    for (int i = 7; i >= 0; --i) {
        b[i] = a & 0x01;
        a >>= 1;
    }
}

static void inttobool16(int a, bool b[])
{
    for (int i = 15; i >= 0; --i) {
        b[i] = a & 0x01;
        a >>= 1;
    }
}

static int todigit(int ch)
{
    return (ch >= '0' && ch <= '9' ? ch - '0' :
            ch >= 'a' && ch <= 'f' ? ch - 'a' + 10 :
            ch >= 'A' && ch <= 'F' ? ch - 'A' + 10 : 0);
}

static int hex2int(int n, trans_line *line_buf)
{
    int result;

    result = 0;
    for (int i = n - 1; i >= 0; --i) {
        if (!isxdigit(*line_buf->line))
            return -1;
        result += pow(16, i) * todigit(*line_buf->line++);
    }
    if (*line_buf->line == 'h' || *line_buf->line == 'H')
        line_buf->line++;

    return result;
}

static void white_space(trans_line *t)
{
    while (isblank(*t->line))
        t->line++;
}

static int getins(trans_line *t, int *addr)
{
    int ins;

    white_space(t);
    if (*t->line == '#') {
        return COMMENT;
    } else if (*t->line == '\n' || *t->line == '\r' || *t->line == '\0') {
        return BLANK_LINE;
    } else if (STRNCASECMP("LOAD", ==, t->line, 4)) {
        t->line += 4;
        ins = LOAD;
    } else if (STRNCASECMP("STORE", ==, t->line, 5)) {
        t->line += 5;
        ins = STORE;
    } else if (STRNCASECMP("ADD", ==, t->line, 3)) {
        t->line += 3;
        ins = ADD;
    } else if (STRNCASECMP("SUB", ==, t->line, 3)) {
        t->line += 3;
        ins = SUB;
    } else if (STRNCASECMP("AWC", ==, t->line, 3)) {
        t->line += 3;
        ins = AWC;
    } else if (STRNCASECMP("SWB", ==, t->line, 3)) {
        t->line += 3;
        ins = SWB;
    } else if (STRNCASECMP("HALT", ==, t->line, 4)) {
        t->line += 4;
        ins = HALT;
    } else {
        if ((*addr = hex2int(4, t)) < 0)
            return -1;
        else
            ins = SAVE;
    }
    if (!isblank(*t->line) && *t->line != '\0'
            && *t->line != '\n' && *t->line != '\r')
        ins = -1;
    else
        t->line++;
    return ins;
}

static void print_addr(int addr, trans_line *t)
{
    bool addr_buf[16];

    inttobool16(addr, addr_buf);
    for (int i = 0; i < 16; ++i)
        sprintf(t->line++, "%1d", addr_buf[i]);
    sprintf(t->line, "\n");
    t->line++;
}

static void print_data(int data, trans_line *t)
{
    bool data_bool[8];

    inttobool8(data, data_bool);
    for (int i = 0; i < 8; ++i)
        sprintf(t->line++, "%1d", data_bool[i]);
    sprintf(t->line, "\n");
    t->line++;
}

static void print_data_reverse(int data, int *addr, trans_line *parse_buf)
{
    bool data_bool[16];

    inttobool16(data, data_bool);
    sprintf(parse_buf->line, "1\n1\n");
    parse_buf->line += 4;
    print_addr(*addr, parse_buf);
    for (int i = 15; i >= 8; --i)
        sprintf(parse_buf->line++, "%1d", data_bool[i]);
    sprintf(parse_buf->line, "\n");
    parse_buf->line++;
    (*addr)++;
    sprintf(parse_buf->line, "1\n1\n");
    parse_buf->line += 4;
    print_addr(*addr, parse_buf);
    for (int i = 7; i >= 0; --i)
        sprintf(parse_buf->line++, "%1d", data_bool[i]);
    sprintf(parse_buf->line, "\n");
    parse_buf->line++;
    (*addr)++;
    sprintf(parse_buf->line, "1\n1\n");
    parse_buf->line += 4;
    print_addr(*addr, parse_buf);
    for (int i = 0; i < 8; ++i)
        sprintf(parse_buf->line++, "%1d", 0);
    sprintf(parse_buf->line, "\n");
    parse_buf->line++;
    (*addr)++;
}

static int parse_line(int *addr, trans_line *line_buf, trans_line *parse_buf)
{
    int  ins;
    int  addr2;
    int  data;

    ins = getins(line_buf, &addr2);
    switch (ins) {
    case LOAD:
    case STORE:
    case ADD:
    case SUB:
    case AWC:
    case SWB:
    case HALT:
        print_addr(*addr, parse_buf);
        print_data(ins, parse_buf);
        (*addr)++;
        if (ins == HALT)
            return 0;
        break;
    case SAVE:
        print_addr(addr2, parse_buf);
        break;
    case COMMENT:
    case BLANK_LINE:
        return -1;
        break;
    default:
        return -2;
    }
    white_space(line_buf);
    if (ins == SAVE) {
        if ((data = hex2int(2, line_buf)) < 0)
            return -2;
        print_data(data, parse_buf);
    } else {
        if ((data = hex2int(4, line_buf)) < 0)
            return -2;
        print_data_reverse(data, addr, parse_buf);
    }

    return 0;
}

static int translator(const char *infile, FILE *outfile)
{
    FILE*       infp;
    char        buf[MAXLINE] = "";
    char        trans_result[MAXLINE];
    trans_line  line_buf, parse_buf;
    int         addr;
    int         ret;

    if((infp = fopen(infile, "r")) == NULL)
        goto bad;

    addr = 0;
    while (fgets(buf, MAXLINE, infp) != NULL) {
        error_line++;
        line_buf.line = buf;
        parse_buf.line = trans_result;
        ret = parse_line(&addr, &line_buf, &parse_buf);
        if (ret == -1)
            continue;
        else if (ret == -2)
            goto bad;
        fprintf(outfile, "1\n1\n");
        fputs(trans_result, outfile);
    }

    fprintf(outfile, "0\n0\n");
    fclose(infp);
    
    return 0;
bad:
    return -1;
}

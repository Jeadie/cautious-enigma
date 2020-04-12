#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char buf[100];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s string\n", argv[0]);
        exit(1);
    }
    strcpy(buf, argv[1]);

    return 0;
}

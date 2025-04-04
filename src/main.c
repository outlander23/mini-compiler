#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }
    FILE *src = fopen(argv[1], "r");
    if (!src) {
        perror("Error opening file");
        return 1;
    }
    yyin = src;
    if (yyparse() == 0) {
        printf("Compilation successful\n");
    } else {
        printf("Compilation failed\n");
    }
    fclose(src);
    return 0;
}
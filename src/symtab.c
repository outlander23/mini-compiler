#include "symtab.h"
#include <stdlib.h>
#include <string.h>

#define MAX_SYM 100

typedef struct {
    char name[32];
    int type;
    int value;
} Symbol;

static Symbol symtab[MAX_SYM];
static int sym_count = 0;

int insert(char *name, int type, int value) {
    if (sym_count >= MAX_SYM) return -1;
    strcpy(symtab[sym_count].name, name);
    symtab[sym_count].type = type;
    symtab[sym_count].value = value;
    sym_count++;
    return 0;
}

Symbol* lookup(char *name) {
    for (int i = 0; i < sym_count; i++) {
        if (strcmp(symtab[i].name, name) == 0) {
            return &symtab[i];
        }
    }
    return NULL;
}

void clear() {
    sym_count = 0;
}
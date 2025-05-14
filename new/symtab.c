#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

#define MAX_VARS 100

typedef struct
{
    char name[32];
    int value;
} Variable;

static Variable symtab[MAX_VARS];
static int symtab_count = 0;

int get_var(const char *name)
{
    for (int i = 0; i < symtab_count; ++i)
        if (strcmp(symtab[i].name, name) == 0)
            return symtab[i].value;
    printf("Error: Undefined variable '%s'\n", name);
    exit(1);
}

void set_var(const char *name, int value)
{
    for (int i = 0; i < symtab_count; ++i)
    {
        if (strcmp(symtab[i].name, name) == 0)
        {
            symtab[i].value = value;
            return;
        }
    }
    if (symtab_count < MAX_VARS)
    {
        strcpy(symtab[symtab_count].name, name);
        symtab[symtab_count].value = value;
        ++symtab_count;
    }
    else
    {
        printf("Symbol table full!\n");
        exit(1);
    }
}
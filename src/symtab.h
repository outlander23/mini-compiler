typedef struct {
    char *name;
    int type;
    int value;
} Symbol;

void insert(char *name, int type, int value);
Symbol *lookup(char *name);
void init_symtab();
#ifndef CODEGEN_H
#define CODEGEN_H

void gen_code(ASTNode *root);
void emit(const char *code);
void init_codegen();

#endif
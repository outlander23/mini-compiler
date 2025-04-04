#include "codegen.h"
#include "ast.h"
#include <stdio.h>

void gen_code(Node *root) {
    if (!root) return;
    switch (root->type) {
        case NODE_NUM:
            printf("PUSH %d\n", root->val);
            break;
        case NODE_VAR:
            printf("LOAD %s\n", root->var_name);
            break;
        case NODE_ADD:
            gen_code(root->left);
            gen_code(root->right);
            printf("ADD\n");
            break;
        case NODE_SUB:
            gen_code(root->left);
            gen_code(root->right);
            printf("SUB\n");
            break;
        case NODE_MUL:
            gen_code(root->left);
            gen_code(root->right);
            printf("MUL\n");
            break;
        case NODE_DIV:
            gen_code(root->left);
            gen_code(root->right);
            printf("DIV\n");
            break;
        case NODE_IF:
            gen_code(root->cond);
            printf("JZ L%d\n", root->label);
            gen_code(root->then_branch);
            printf("L%d:\n", root->label);
            break;
        case NODE_WHILE:
            printf("L%d:\n", root->label);
            gen_code(root->cond);
            printf("JZ L%d\n", root->end_label);
            gen_code(root->body);
            printf("JMP L%d\n", root->label);
            printf("L%d:\n", root->end_label);
            break;
        case NODE_FOR:
            gen_code(root->init);
            printf("L%d:\n", root->label);
            gen_code(root->cond);
            printf("JZ L%d\n", root->end_label);
            gen_code(root->body);
            gen_code(root->update);
            printf("JMP L%d\n", root->label);
            printf("L%d:\n", root->end_label);
            break;
    }
}
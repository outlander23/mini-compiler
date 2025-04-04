#include "ast.h"
#include <stdlib.h>

Node* new_node(int type, Node* left, Node* right) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->type = type;
    n->left = left;
    n->right = right;
    return n;
}

void free_node(Node* n) {
    if (n) {
        free_node(n->left);
        free_node(n->right);
        free(n);
    }
}

void print_ast(Node* n, int depth) {
    if (!n) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("Node Type: %d\n", n->type);
    print_ast(n->left, depth + 1);
    print_ast(n->right, depth + 1);
}
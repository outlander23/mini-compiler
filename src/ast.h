typedef struct ASTNode {
    int type;
    struct ASTNode *left;
    struct ASTNode *right;
    int value;
} ASTNode;

ASTNode* createNode(int type, ASTNode* left, ASTNode* right, int value);
void freeAST(ASTNode* node);
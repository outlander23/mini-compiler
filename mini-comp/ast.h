#ifndef AST_H
#define AST_H

// Node types
typedef enum
{
    NODE_PROGRAM,
    NODE_STMT_LIST,
    NODE_IF_STMT,
    NODE_FOR_STMT,
    NODE_WHILE_STMT,
    NODE_ASSIGN_STMT,
    NODE_EXPR,
    NODE_BINARY_EXPR,
    NODE_UNARY_EXPR,
    NODE_INT_LITERAL,
    NODE_FLOAT_LITERAL,
    NODE_IDENTIFIER,
    NODE_EMPTY
} NodeType;

// Expression types
typedef enum
{
    EXPR_PLUS,
    EXPR_MINUS,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_MOD,
    EXPR_EQ,
    EXPR_NEQ,
    EXPR_LT,
    EXPR_LE,
    EXPR_GT,
    EXPR_GE,
    EXPR_AND,
    EXPR_OR,
    EXPR_NOT,
    EXPR_NEG,
    EXPR_INT,
    EXPR_FLOAT,
    EXPR_ID
} ExprType;

// Forward declaration
typedef struct AST AST;

// AST structure
struct AST
{
    NodeType type;
    ExprType expr_type;
    union
    {
        int int_val;
        float float_val;
        char *str_val;

        // For binary expressions
        struct
        {
            AST *left;
            AST *right;
        } binary;

        // For unary expressions
        struct
        {
            AST *operand;
        } unary;

        // For if statements
        struct
        {
            AST *condition;
            AST *then_block;
            AST *else_block; // May be NULL
        } if_stmt;

        // For for statements
        struct
        {
            AST *init;
            AST *condition;
            AST *update;
            AST *body;
        } for_stmt;

        // For while statements
        struct
        {
            AST *condition;
            AST *body;
        } while_stmt;

        // For assignment statements
        struct
        {
            AST *left;
            AST *right;
        } assign_stmt;

        // For statement lists
        struct
        {
            int count;
            int capacity;
            AST **statements;
        } stmt_list;
    };
};

// Function declarations
AST *create_int_node(int value);
AST *create_float_node(float value);
AST *create_identifier_node(char *name);
AST *create_binary_expr_node(ExprType type, AST *left, AST *right);
AST *create_unary_expr_node(ExprType type, AST *operand);
AST *create_if_node(AST *condition, AST *then_block, AST *else_block);
AST *create_for_node(AST *init, AST *condition, AST *update, AST *body);
AST *create_while_node(AST *condition, AST *body);
AST *create_assign_node(AST *left, AST *right);
AST *create_stmt_list_node();
void add_statement(AST *list, AST *statement);
void free_ast(AST *node);

// Evaluation and code generation
int eval_int_expr(AST *expr);
float eval_float_expr(AST *expr);
void generate_intermediate_code(AST *node);

#endif // AST_H
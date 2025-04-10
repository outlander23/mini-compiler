#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Create an integer literal node
AST *create_int_node(int value)
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = NODE_INT_LITERAL;
    node->expr_type = EXPR_INT;
    node->int_val = value;
    return node;
}

// Create a float literal node
AST *create_float_node(float value)
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = NODE_FLOAT_LITERAL;
    node->expr_type = EXPR_FLOAT;
    node->float_val = value;
    return node;
}

// Create an identifier node
AST *create_identifier_node(char *name)
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = NODE_IDENTIFIER;
    node->expr_type = EXPR_ID;
    node->str_val = strdup(name);
    return node;
}

// Create a binary expression node
AST *create_binary_expr_node(ExprType type, AST *left, AST *right)
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = NODE_BINARY_EXPR;
    node->expr_type = type;
    node->binary.left = left;
    node->binary.right = right;
    return node;
}

// Create a unary expression node
AST *create_unary_expr_node(ExprType type, AST *operand)
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = NODE_UNARY_EXPR;
    node->expr_type = type;
    node->unary.operand = operand;
    return node;
}

// Create an if statement node
AST *create_if_node(AST *condition, AST *then_block, AST *else_block)
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = NODE_IF_STMT;
    node->if_stmt.condition = condition;
    node->if_stmt.then_block = then_block;
    node->if_stmt.else_block = else_block;
    return node;
}

// Create a for statement node
AST *create_for_node(AST *init, AST *condition, AST *update, AST *body)
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = NODE_FOR_STMT;
    node->for_stmt.init = init;
    node->for_stmt.condition = condition;
    node->for_stmt.update = update;
    node->for_stmt.body = body;
    return node;
}

// Create a while statement node
AST *create_while_node(AST *condition, AST *body)
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = NODE_WHILE_STMT;
    node->while_stmt.condition = condition;
    node->while_stmt.body = body;
    return node;
}

// Create an assignment statement node
AST *create_assign_node(AST *left, AST *right)
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = NODE_ASSIGN_STMT;
    node->assign_stmt.left = left;
    node->assign_stmt.right = right;
    return node;
}

// Create a statement list node
AST *create_stmt_list_node()
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = NODE_STMT_LIST;
    node->stmt_list.count = 0;
    node->stmt_list.capacity = 10;
    node->stmt_list.statements = (AST **)malloc(sizeof(AST *) * node->stmt_list.capacity);
    return node;
}

// Add a statement to a statement list
void add_statement(AST *list, AST *statement)
{
    if (list->type != NODE_STMT_LIST)
    {
        fprintf(stderr, "Error: Not a statement list\n");
        return;
    }

    if (list->stmt_list.count >= list->stmt_list.capacity)
    {
        list->stmt_list.capacity *= 2;
        list->stmt_list.statements = (AST **)realloc(list->stmt_list.statements,
                                                     sizeof(AST *) * list->stmt_list.capacity);
    }

    list->stmt_list.statements[list->stmt_list.count++] = statement;
}

// Free memory allocated for AST
void free_ast(AST *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
    case NODE_IDENTIFIER:
        free(node->str_val);
        break;

    case NODE_BINARY_EXPR:
        free_ast(node->binary.left);
        free_ast(node->binary.right);
        break;

    case NODE_UNARY_EXPR:
        free_ast(node->unary.operand);
        break;

    case NODE_IF_STMT:
        free_ast(node->if_stmt.condition);
        free_ast(node->if_stmt.then_block);
        if (node->if_stmt.else_block)
            free_ast(node->if_stmt.else_block);
        break;

    case NODE_FOR_STMT:
        free_ast(node->for_stmt.init);
        free_ast(node->for_stmt.condition);
        free_ast(node->for_stmt.update);
        free_ast(node->for_stmt.body);
        break;

    case NODE_WHILE_STMT:
        free_ast(node->while_stmt.condition);
        free_ast(node->while_stmt.body);
        break;

    case NODE_ASSIGN_STMT:
        free_ast(node->assign_stmt.left);
        free_ast(node->assign_stmt.right);
        break;

    case NODE_STMT_LIST:
        for (int i = 0; i < node->stmt_list.count; i++)
        {
            free_ast(node->stmt_list.statements[i]);
        }
        free(node->stmt_list.statements);
        break;

    default:
        break;
    }

    free(node);
}

// Simple symbol table for evaluation
#define MAX_SYMBOLS 100
struct
{
    char *name;
    union
    {
        int int_val;
        float float_val;
    };
    int is_float;
} symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

// Helper functions for symbol table
int find_symbol(char *name)
{
    for (int i = 0; i < symbol_count; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
            return i;
    }
    return -1;
}

void add_int_symbol(char *name, int value)
{
    int idx = find_symbol(name);
    if (idx >= 0)
    {
        symbol_table[idx].int_val = value;
        symbol_table[idx].is_float = 0;
        return;
    }

    if (symbol_count < MAX_SYMBOLS)
    {
        symbol_table[symbol_count].name = strdup(name);
        symbol_table[symbol_count].int_val = value;
        symbol_table[symbol_count].is_float = 0;
        symbol_count++;
    }
}

void add_float_symbol(char *name, float value)
{
    int idx = find_symbol(name);
    if (idx >= 0)
    {
        symbol_table[idx].float_val = value;
        symbol_table[idx].is_float = 1;
        return;
    }

    if (symbol_count < MAX_SYMBOLS)
    {
        symbol_table[symbol_count].name = strdup(name);
        symbol_table[symbol_count].float_val = value;
        symbol_table[symbol_count].is_float = 1;
        symbol_count++;
    }
}

// Function to evaluate integer expressions
int eval_int_expr(AST *expr)
{
    if (!expr)
        return 0;

    switch (expr->type)
    {
    case NODE_INT_LITERAL:
        return expr->int_val;

    case NODE_IDENTIFIER:
    {
        int idx = find_symbol(expr->str_val);
        if (idx >= 0 && !symbol_table[idx].is_float)
        {
            return symbol_table[idx].int_val;
        }
        fprintf(stderr, "Error: Undefined integer variable %s\n", expr->str_val);
        return 0;
    }

    case NODE_BINARY_EXPR:
        switch (expr->expr_type)
        {
        case EXPR_PLUS:
            return eval_int_expr(expr->binary.left) + eval_int_expr(expr->binary.right);
        case EXPR_MINUS:
            return eval_int_expr(expr->binary.left) - eval_int_expr(expr->binary.right);
        case EXPR_MUL:
            return eval_int_expr(expr->binary.left) * eval_int_expr(expr->binary.right);
        case EXPR_DIV:
        {
            int divisor = eval_int_expr(expr->binary.right);
            if (divisor == 0)
            {
                fprintf(stderr, "Error: Division by zero\n");
                return 0;
            }
            return eval_int_expr(expr->binary.left) / divisor;
        }
        case EXPR_MOD:
        {
            int divisor = eval_int_expr(expr->binary.right);
            if (divisor == 0)
            {
                fprintf(stderr, "Error: Modulo by zero\n");
                return 0;
            }
            return eval_int_expr(expr->binary.left) % divisor;
        }
        case EXPR_EQ:
            return eval_int_expr(expr->binary.left) == eval_int_expr(expr->binary.right);
        case EXPR_NEQ:
            return eval_int_expr(expr->binary.left) != eval_int_expr(expr->binary.right);
        case EXPR_LT:
            return eval_int_expr(expr->binary.left) < eval_int_expr(expr->binary.right);
        case EXPR_LE:
            return eval_int_expr(expr->binary.left) <= eval_int_expr(expr->binary.right);
        case EXPR_GT:
            return eval_int_expr(expr->binary.left) > eval_int_expr(expr->binary.right);
        case EXPR_GE:
            return eval_int_expr(expr->binary.left) >= eval_int_expr(expr->binary.right);
        case EXPR_AND:
            return eval_int_expr(expr->binary.left) && eval_int_expr(expr->binary.right);
        case EXPR_OR:
            return eval_int_expr(expr->binary.left) || eval_int_expr(expr->binary.right);
        default:
            fprintf(stderr, "Error: Unsupported binary operation for integer\n");
            return 0;
        }

    case NODE_UNARY_EXPR:
        switch (expr->expr_type)
        {
        case EXPR_NEG:
            return -eval_int_expr(expr->unary.operand);
        case EXPR_NOT:
            return !eval_int_expr(expr->unary.operand);
        default:
            fprintf(stderr, "Error: Unsupported unary operation for integer\n");
            return 0;
        }

    default:
        fprintf(stderr, "Error: Cannot evaluate non-expression node as integer\n");
        return 0;
    }
}

// Function to evaluate float expressions
float eval_float_expr(AST *expr)
{
    if (!expr)
        return 0.0f;

    switch (expr->type)
    {
    case NODE_INT_LITERAL:
        return (float)expr->int_val;

    case NODE_FLOAT_LITERAL:
        return expr->float_val;

    case NODE_IDENTIFIER:
    {
        int idx = find_symbol(expr->str_val);
        if (idx >= 0)
        {
            if (symbol_table[idx].is_float)
                return symbol_table[idx].float_val;
            else
                return (float)symbol_table[idx].int_val;
        }
        fprintf(stderr, "Error: Undefined float variable %s\n", expr->str_val);
        return 0.0f;
    }

    case NODE_BINARY_EXPR:
        switch (expr->expr_type)
        {
        case EXPR_PLUS:
            return eval_float_expr(expr->binary.left) + eval_float_expr(expr->binary.right);
        case EXPR_MINUS:
            return eval_float_expr(expr->binary.left) - eval_float_expr(expr->binary.right);
        case EXPR_MUL:
            return eval_float_expr(expr->binary.left) * eval_float_expr(expr->binary.right);
        case EXPR_DIV:
        {
            float divisor = eval_float_expr(expr->binary.right);
            if (divisor == 0.0f)
            {
                fprintf(stderr, "Error: Division by zero\n");
                return 0.0f;
            }
            return eval_float_expr(expr->binary.left) / divisor;
        }
        case EXPR_EQ:
            return eval_float_expr(expr->binary.left) == eval_float_expr(expr->binary.right);
        case EXPR_NEQ:
            return eval_float_expr(expr->binary.left) != eval_float_expr(expr->binary.right);
        case EXPR_LT:
            return eval_float_expr(expr->binary.left) < eval_float_expr(expr->binary.right);
        case EXPR_LE:
            return eval_float_expr(expr->binary.left) <= eval_float_expr(expr->binary.right);
        case EXPR_GT:
            return eval_float_expr(expr->binary.left) > eval_float_expr(expr->binary.right);
        case EXPR_GE:
            return eval_float_expr(expr->binary.left) >= eval_float_expr(expr->binary.right);
        default:
            fprintf(stderr, "Error: Unsupported binary operation for float\n");
            return 0.0f;
        }

    case NODE_UNARY_EXPR:
        switch (expr->expr_type)
        {
        case EXPR_NEG:
            return -eval_float_expr(expr->unary.operand);
        default:
            fprintf(stderr, "Error: Unsupported unary operation for float\n");
            return 0.0f;
        }

    default:
        fprintf(stderr, "Error: Cannot evaluate non-expression node as float\n");
        return 0.0f;
    }
}

// Intermediate code generation
int temp_var_count = 0;
int label_count = 0;

char *get_temp_var()
{
    char *temp = (char *)malloc(10);
    sprintf(temp, "t%d", temp_var_count++);
    return temp;
}

char *get_label()
{
    char *label = (char *)malloc(10);
    sprintf(label, "L%d", label_count++);
    return label;
}

void generate_expr_code(AST *expr)
{
    if (!expr)
        return;

    char *result;
    char *left_result;
    char *right_result;
    char *true_label;
    char *false_label;
    char *next_label;

    switch (expr->type)
    {
    case NODE_INT_LITERAL:
        result = get_temp_var();
        printf("%s = %d\n", result, expr->int_val);
        free(result);
        break;

    case NODE_FLOAT_LITERAL:
        result = get_temp_var();
        printf("%s = %f\n", result, expr->float_val);
        free(result);
        break;

    case NODE_IDENTIFIER:
        // For identifiers, we don't need to generate code
        break;

    case NODE_BINARY_EXPR:
        left_result = get_temp_var();
        right_result = get_temp_var();
        result = get_temp_var();

        // Generate code for operands
        generate_expr_code(expr->binary.left);
        generate_expr_code(expr->binary.right);

        // Use variable names if identifiers
        if (expr->binary.left->type == NODE_IDENTIFIER)
            left_result = expr->binary.left->str_val;

        if (expr->binary.right->type == NODE_IDENTIFIER)
            right_result = expr->binary.right->str_val;

        // Generate operation code
        switch (expr->expr_type)
        {
        case EXPR_PLUS:
            printf("%s = %s + %s\n", result, left_result, right_result);
            break;
        case EXPR_MINUS:
            printf("%s = %s - %s\n", result, left_result, right_result);
            break;
        case EXPR_MUL:
            printf("%s = %s * %s\n", result, left_result, right_result);
            break;
        case EXPR_DIV:
            printf("%s = %s / %s\n", result, left_result, right_result);
            break;
        case EXPR_MOD:
            printf("%s = %s %% %s\n", result, left_result, right_result);
            break;
        case EXPR_EQ:
            printf("%s = %s == %s\n", result, left_result, right_result);
            break;
        case EXPR_NEQ:
            printf("%s = %s != %s\n", result, left_result, right_result);
            break;
        case EXPR_LT:
            printf("%s = %s < %s\n", result, left_result, right_result);
            break;
        case EXPR_LE:
            printf("%s = %s <= %s\n", result, left_result, right_result);
            break;
        case EXPR_GT:
            printf("%s = %s > %s\n", result, left_result, right_result);
            break;
        case EXPR_GE:
            printf("%s = %s >= %s\n", result, left_result, right_result);
            break;
        case EXPR_AND:
            printf("%s = %s && %s\n", result, left_result, right_result);
            break;
        case EXPR_OR:
            printf("%s = %s || %s\n", result, left_result, right_result);
            break;
        default:
            break;
        }

        if (expr->binary.left->type != NODE_IDENTIFIER)
            free(left_result);
        if (expr->binary.right->type != NODE_IDENTIFIER)
            free(right_result);
        free(result);
        break;

    case NODE_UNARY_EXPR:
        result = get_temp_var();
        generate_expr_code(expr->unary.operand);

        if (expr->unary.operand->type == NODE_IDENTIFIER)
            right_result = expr->unary.operand->str_val;
        else
            right_result = get_temp_var();

        switch (expr->expr_type)
        {
        case EXPR_NEG:
            printf("%s = -%s\n", result, right_result);
            break;
        case EXPR_NOT:
            printf("%s = !%s\n", result, right_result);
            break;
        default:
            break;
        }

        if (expr->unary.operand->type != NODE_IDENTIFIER)
            free(right_result);
        free(result);
        break;

    default:
        break;
    }
}

void generate_intermediate_code(AST *node)
{
    if (!node)
        return;

    char *cond_result;
    char *true_label;
    char *false_label;
    char *next_label;
    char *loop_start;
    char *loop_end;

    switch (node->type)
    {
    case NODE_PROGRAM:
        // Program shouldn't appear directly in the AST as we've designed it
        break;

    case NODE_STMT_LIST:
        for (int i = 0; i < node->stmt_list.count; i++)
        {
            printf("// Statement %d\n", i + 1);
            generate_intermediate_code(node->stmt_list.statements[i]);
        }
        break;

    case NODE_IF_STMT:
        true_label = get_label();
        false_label = node->if_stmt.else_block ? get_label() : NULL;
        next_label = get_label();

        printf("// If condition\n");
        generate_expr_code(node->if_stmt.condition);
        cond_result = get_temp_var();

        // For simple conditions like identifiers
        if (node->if_stmt.condition->type == NODE_IDENTIFIER)
            cond_result = node->if_stmt.condition->str_val;

        if (node->if_stmt.else_block)
        {
            printf("if %s goto %s\n", cond_result, true_label);
            printf("goto %s\n", false_label);
            printf("%s:\n", true_label);
            printf("// Then block\n");
            generate_intermediate_code(node->if_stmt.then_block);
            printf("goto %s\n", next_label);
            printf("%s:\n", false_label);
            printf("// Else block\n");
            generate_intermediate_code(node->if_stmt.else_block);
            printf("%s:\n", next_label);
        }
        else
        {
            printf("if %s goto %s\n", cond_result, true_label);
            printf("goto %s\n", next_label);
            printf("%s:\n", true_label);
            printf("// Then block\n");
            generate_intermediate_code(node->if_stmt.then_block);
            printf("%s:\n", next_label);
        }

        if (node->if_stmt.condition->type != NODE_IDENTIFIER)
            free(cond_result);
        free(true_label);
        if (false_label)
            free(false_label);
        free(next_label);
        break;

    case NODE_FOR_STMT:
        loop_start = get_label();
        loop_end = get_label();

        printf("// For initialization\n");
        generate_intermediate_code(node->for_stmt.init);

        printf("%s:\n", loop_start);
        printf("// For condition\n");
        generate_expr_code(node->for_stmt.condition);
        cond_result = get_temp_var();

        // For simple conditions like identifiers
        if (node->for_stmt.condition->type == NODE_IDENTIFIER)
            cond_result = node->for_stmt.condition->str_val;

        printf("if !%s goto %s\n", cond_result, loop_end);

        printf("// For body\n");
        generate_intermediate_code(node->for_stmt.body);

        printf("// For update\n");
        generate_intermediate_code(node->for_stmt.update);

        printf("goto %s\n", loop_start);
        printf("%s:\n", loop_end);

        if (node->for_stmt.condition->type != NODE_IDENTIFIER)
            free(cond_result);
        free(loop_start);
        free(loop_end);
        break;

    case NODE_WHILE_STMT:
        loop_start = get_label();
        loop_end = get_label();

        printf("%s:\n", loop_start);
        printf("// While condition\n");
        generate_expr_code(node->while_stmt.condition);
        cond_result = get_temp_var();

        // For simple conditions like identifiers
        if (node->while_stmt.condition->type == NODE_IDENTIFIER)
            cond_result = node->while_stmt.condition->str_val;

        printf("if !%s goto %s\n", cond_result, loop_end);

        printf("// While body\n");
        generate_intermediate_code(node->while_stmt.body);

        printf("goto %s\n", loop_start);
        printf("%s:\n", loop_end);

        if (node->while_stmt.condition->type != NODE_IDENTIFIER)
            free(cond_result);
        free(loop_start);
        free(loop_end);
        break;

    case NODE_ASSIGN_STMT:
        printf("// Assignment\n");
        generate_expr_code(node->assign_stmt.right);

        if (node->assign_stmt.left->type == NODE_IDENTIFIER)
        {
            char *right_result;

            if (node->assign_stmt.right->type == NODE_IDENTIFIER)
                right_result = node->assign_stmt.right->str_val;
            else
            {
                right_result = get_temp_var();
                // This is simplified, as we'd need more complex code to get the actual value
            }

            printf("%s = %s\n", node->assign_stmt.left->str_val, right_result);

            if (node->assign_stmt.right->type != NODE_IDENTIFIER)
                free(right_result);
        }
        break;

    case NODE_EXPR:
        generate_expr_code(node);
        break;

    default:
        break;
    }
}
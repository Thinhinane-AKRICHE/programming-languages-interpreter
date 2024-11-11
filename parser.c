#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"
#include "lexer.c"
// Initialisation de la pile
void initialize(PILE *pile) {
    pile->TOP = -1;
}

int isEmpty(PILE *pile) {
    return pile->TOP == -1;
}

// Vérification si la pile est pleine pour ne pas dépasser sa capacité max
int isFull(PILE *pile) {
    return pile->TOP == MAX - 1;
}

// Fonction pour ajouter un élément dans la pile
void push(PILE *pile, int value) {
    if (isFull(pile)) {
        printf("Erreur: la pile est pleine\n");
    } else {
        pile->TOP++;
        pile->pile[pile->TOP] = value;
    }
}

// Fonction pour retirer un élément de la pile
int pop(PILE *pile) {
    if (isEmpty(pile)) {
        printf("Erreur: la pile est vide\n");
        return -1;
    } else {
        int value = pile->pile[pile->TOP];
        pile->TOP--;
        return value;
    }
}

// Fonction pour obtenir un élément sans le retirer
int peek(PILE *pile) {
    if (isEmpty(pile)) {
        printf("Erreur: la pile est vide\n");
        return -1;
    } else {
        return pile->pile[pile->TOP];
    }
}

// Initialisation de la pile AST
void initializeASTStack(ASTStack *stack) {
    stack->top = -1;
}

int isEmptyAST(ASTStack *stack) {
    return stack->top == -1;
}

// Ajoute un nœud dans la pile AST
void pushAST(ASTStack *stack, ASTNode *node) {
    if (stack->top < MAX - 1) {
        stack->nodes[++stack->top] = node;
    }
}

// Retire un nœud de la pile AST
ASTNode *popAST(ASTStack *stack) {
    return stack->top >= 0 ? stack->nodes[stack->top--] : NULL;
}

ASTNode *create_node(TokenType type, const char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Fonction de priorité des opérateurs
int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Fonction parse pour construire l'AST avec Shunting Yard
ASTNode *parse(Token *tokens, int num_tokens) {
    PILE operatorStack;
    ASTStack astStack;
    initialize(&operatorStack);
    initializeASTStack(&astStack);

    for (int i = 0; i < num_tokens; i++) {
        Token token = tokens[i];

        if (token.type == TOKEN_NUMBER || token.type == TOKEN_VARIABLE) {
            ASTNode *node = create_node(TOKEN_NUMBER, token.value);
            pushAST(&astStack, node);
        }
        else if (token.type == TOKEN_OPERATOR) {
            while (!isEmpty(&operatorStack) && priority(peek(&operatorStack)) >= priority(token.value[0])) {
                char operator = pop(&operatorStack);
                ASTNode *right = popAST(&astStack);
                ASTNode *left = popAST(&astStack);
                ASTNode *operatorNode = create_node(TOKEN_OPERATOR, (char[]){operator, '\0'});
                operatorNode->left = left;
                operatorNode->right = right;
                pushAST(&astStack, operatorNode);
            }
            push(&operatorStack, token.value[0]);
        }
        else if (token.type == TOKEN_PARENTHESIS) {
            if (token.value[0] == '(') {
                push(&operatorStack, token.value[0]);
            } else {
                while (!isEmpty(&operatorStack) && peek(&operatorStack) != '(') {
                    char operator = pop(&operatorStack);
                    ASTNode *right = popAST(&astStack);
                    ASTNode *left = popAST(&astStack);
                    ASTNode *operatorNode = create_node(TOKEN_OPERATOR, (char[]){operator, '\0'});
                    operatorNode->left = left;
                    operatorNode->right = right;
                    pushAST(&astStack, operatorNode);
                }
                pop(&operatorStack); // Retirer '('
            }
        }
    }

    while (!isEmpty(&operatorStack)) {
        char operator = pop(&operatorStack);
        ASTNode *right = popAST(&astStack);
        ASTNode *left = popAST(&astStack);
        ASTNode *operatorNode = create_node(TOKEN_OPERATOR, (char[]){operator, '\0'});
        operatorNode->left = left;
        operatorNode->right = right;
        pushAST(&astStack, operatorNode);
    }

    return popAST(&astStack);
}

// Fonction récursive pour évaluer l'AST
int evaluate_AST(ASTNode *node) {
    if (node->type == TOKEN_NUMBER) {
        return atoi(node->value);
    } else if (node->type == TOKEN_OPERATOR) {
        int left_val = evaluate_AST(node->left);
        int right_val = evaluate_AST(node->right);

        switch (node->value[0]) {
            case '+': return left_val + right_val;
            case '-': return left_val - right_val;
            case '*': return left_val * right_val;
            case '/': return right_val != 0 ? left_val / right_val : 0;
            default: printf("Opérateur inconnu\n"); return 0;
        }
    }
    return 0;
}

// Fonction pour libérer l'AST
void free_AST(ASTNode *node) {
    if (node) {
        free_AST(node->left);
        free_AST(node->right);
        free(node->value);
        free(node);
    }
}

// Fonction principale
int main() {
    const char *input = "3+5*(2-3+2*4)-9";
    Token *tokens = lexer(input);  // Lexer analyse l'entrée et renvoie les tokens
    int num_tokens = get_token_count();  // Nombre de tokens générés

    ASTNode *ast = parse(tokens, num_tokens);  // Construction de l'AST
    int result = evaluate_AST(ast);  // Évaluation de l'AST
    printf("Résultat de l'expression '%s' : %d\n", input, result);

    free_AST(ast);  // Libérer la mémoire de l'AST
    free_tokens();  // Libérer la mémoire des tokens

    return 0;
}


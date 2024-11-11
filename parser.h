#ifndef PARSER_H
#define PARSER_H
#define MAX 100 // taille max de la pile

typedef struct {
    int pile[MAX]; // tableau pour stocker les elmts de la pile
    int TOP; // Sommet de la pile
}PILE;
typedef struct ASTNode {
    TokenType type;
    char *value;            // Stocke un nombre ou un opérateur
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;
typedef struct {
    ASTNode *nodes[MAX]; // Pour stocker les nœuds de l'AST
    int top;
} ASTStack;

void initialize();
int isEmpty(PILE *pile);
int isFull(PILE *pile);
void push();
int pop(PILE *pile);
int peek(PILE *pile);
void initializeASTStack(ASTStack *stack);
int isEmptyAST(ASTStack *stack);
void pushAST(ASTStack *stack, ASTNode *node);
ASTNode *popAST(ASTStack *stack);

#endif //PARSER_H


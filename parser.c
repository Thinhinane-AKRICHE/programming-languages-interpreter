#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"




// Initialisation de la pile
void initialize(PILE *pile) {
    pile->TOP = -1; // Le TOP est à -1 signifie que la pile est vide
    // si on ajoute un elmt on incrémente top et devient
}

int isEmpty(PILE *pile) {
    return pile->TOP ==-1;
}

// Vérification si la pile est pleine pour ne pas dépasser sa capacité max
 int isFull(PILE *pile) {
    return pile->TOP == MAX - 1; // si le sommet est à MAX-1 la pile est pleine
    // MAX - 1 : pour nous assurer que nous restons dans les limites de notre tableau , exple: si MAX est 10 les indices vont de 0 a 9

}

// Fonction pour ajouter un elmt dans la pile
void push(PILE *pile, int value) {
    if (isFull(pile)) {
        // Vérifier si la pile est pleine
        printf("Erreur: pile is full\n");
    }else {
        pile->TOP++; // On incrémente le sommet pour ajouter un nouvel elmt
        pile->pile[pile->TOP] = value; // placer la valeur à l'indice du sommet
        printf("Ajouté : %d\n" , value);
    }
}

//Fonction pour retirer un elmt de la pile
int pop(PILE *pile) {
    if (isEmpty(pile)) {
        // On vérifie si la pile est vide
        printf ("Erreur: pile is empty\n");
        return -1; // Valeur d'erreur
    }else {
        int value = pile -> pile[pile->TOP]; //
        pile->TOP--; // on décrémente le sommet
        printf("Retiré : %d\n" , value);
        return value; // Retourner la valeur retirée
    }
}

// Fonction pour obtenir un elmt sans le retirer

int peek(PILE *pile) {
    if (isEmpty(pile)) {
        printf ("Erreur: pile is empty\n");
        return -1;
    } else {
        return pile -> pile[pile->TOP]; // elle utilise le pointeur pile pour récupérer la valeur du sommet du tabbleau
    }
}
// Fonction pour initialiser la pile AST
void initializeASTStack(ASTStack *stack) {
    stack->top = -1;
}

// Vérifie si la pile AST est vide
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
    node->value = strdup(value);  // Copier la valeur dans une nouvelle mémoire
    node->left = NULL;
    node->right = NULL;
    return node;
}



void parse(Token *tokens, int num_tokens) {
    PILE operatorStack; // Utilisée pour le Shunting Yard
    ASTStack astStack;  // Utilisée pour les nœuds de l'AST
    initialize(&operatorStack);
    initializeASTStack(&astStack);

    // Parcours des tokens générés par le lexer
    for (int i = 0; i < num_tokens; i++) {
        Token token = tokens[i]; // token est une variable locale pour stocker le tokens[i]

        if (token.type == TOKEN_NUMBER || token.type == TOKEN_VARIABLE) {
            ASTNode *node = create_node(TOKEN_NUMBER, token.value); // à ce niveau on créer un noeud de type ASTNode et on utilise node comme pointeur vers ce noued 
            pushAST(&astStack, node);
        }
        else if (token.type == TOKEN_OPERATOR) {
           // Gestion des priorités des opérateurs avec Shunting Yard
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
                // Gestion des parenthèses fermantes
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

    // Déplacer les opérateurs restants de la pile vers la sortie
    while (!isEmpty(&operatorStack)) {
        char operator = pop(&operatorStack);
        ASTNode *right = popAST(&astStack);
        ASTNode *left = popAST(&astStack);
        ASTNode *operatorNode = create_node(TOKEN_OPERATOR, (char[]){operator, '\0'});
        operatorNode->left = left;
        operatorNode->right = right;
        pushAST(&astStack, operatorNode);
    }
    // L'AST final est maintenant sur le sommet de la pile AST
    ASTNode *astRoot = popAST(&astStack);
}

// Fonction de priorité des opérateurs
int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}




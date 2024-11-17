#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"
#include "lexer.c"
#include "hash.c"
#include "hash_table.h"


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
    if (op == '<' || op == '>' || op == '=' || op == '!') return 3;
    return 0;
}
#include "parser.h"

// Toutes vos structures et définitions ici...

// Fonction pour afficher l'AST
void print_AST(ASTNode *node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");
    printf("Node: type=%d, value=%s\n", node->type, node->value ? node->value : "NULL");
    if (node->left) print_AST(node->left, indent + 1);
    if (node->right) print_AST(node->right, indent + 1);
}


// Vos autres fonctions comme parse, evaluate_AST...

// Fonction parse pour construire l'AST avec Shunting Yard
ASTNode *parse(Token *tokens, int num_tokens) {
    PILE operatorStack;
    ASTStack astStack;
    initialize(&operatorStack);
    initializeASTStack(&astStack);

    for (int i = 0; i < num_tokens; i++) {
        Token token = tokens[i];

        if (token.type == TOKEN_NUMBER || token.type == TOKEN_VARIABLE) {
            ASTNode *node = create_node(token.type, token.value);
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
        else if (token.type == TOKEN_ASSIGN) {
            if (i > 0 && tokens[i-1].type == TOKEN_VARIABLE) {
                ASTNode *left = popAST(&astStack);
                ASTNode *right = parse(&tokens[i+1], num_tokens - i - 1);
                ASTNode *assignNode = create_node(TOKEN_ASSIGN, "=");
                assignNode->left = left;
                assignNode->right = right;
                pushAST(&astStack, assignNode);
                break;  // Arrêtez le parsing ici car nous avons traité toute l'expression
            } else {
                printf("Erreur de syntaxe : assignation invalide\n");
                return NULL;
            }
        }
    else if (token.type == TOKEN_WHILE) {
    // Créer un nœud pour la boucle while
    ASTNode *whileNode = create_node(TOKEN_WHILE, "while");

    // Parser la condition
    int j = i + 2;  // Assumer que la condition commence après "while("
    int condition_start = j;
    while (tokens[j].type != TOKEN_PARENTHESIS || tokens[j].value[0] != ')') j++; // Trouver la parenthèse fermante
    ASTNode *condition = parse(&tokens[condition_start], j - condition_start);  // Nœud de condition

    // Maintenant, traiter le corps de la boucle
    if (tokens[j + 1].type == TOKEN_BRACE_OPEN) {
        int body_start = j + 2;
        int body_end = body_start;
        int brace_count = 1;
        while (brace_count > 0) {
            body_end++;
            if (tokens[body_end].type == TOKEN_BRACE_OPEN) brace_count++;
            else if (tokens[body_end].type == TOKEN_BRACE_CLOSE) brace_count--;
        }
        ASTNode *body = parse(&tokens[body_start], body_end - body_start);  // Nœud du corps

        // Lier la condition et le corps au nœud while
        whileNode->left = condition;
        whileNode->right = body;

        // Empiler ce nœud dans l'AST
        pushAST(&astStack, whileNode);
        i = body_end; // Avancer après le corps de la boucle
    }
}


else if (token.type == TOKEN_FOR) {
    // Gestion du mot-clé `for`
    if (tokens[i + 1].type == TOKEN_PARENTHESIS && tokens[i + 1].value[0] == '(') {
        int j = i + 2;
        int init_start = j;
        // Identifier les parties de la boucle for (init; condition; incrément)
        while (tokens[j].type != TOKEN_OPERATOR || tokens[j].value[0] != ';') j++;
        ASTNode *init = parse(&tokens[init_start], j - init_start);
        
        int condition_start = j + 1;
        while (tokens[j].type != TOKEN_OPERATOR || tokens[j].value[0] != ';') j++;
        ASTNode *condition = parse(&tokens[condition_start], j - condition_start);
        
        int increment_start = j + 1;
        while (tokens[j].type != TOKEN_PARENTHESIS || tokens[j].value[0] != ')') j++;
        ASTNode *increment = parse(&tokens[increment_start], j - increment_start);
        
        // Identifier le corps de la boucle
        if (tokens[j + 1].type == TOKEN_BRACE_OPEN) {
            int body_start = j + 2;
            int body_end = body_start;
            int brace_count = 1;
            while (brace_count > 0) {
                body_end++;
                if (tokens[body_end].type == TOKEN_BRACE_OPEN) brace_count++;
                else if (tokens[body_end].type == TOKEN_BRACE_CLOSE) brace_count--;
            }
            ASTNode *body = parse(&tokens[body_start], body_end - body_start);
            
            ASTNode *forNode = create_node(TOKEN_FOR, "for");
            forNode->left = init;
            forNode->right = condition;
            forNode->extra = increment;
            forNode->right->right = body; // Le corps de la boucle
            
            pushAST(&astStack, forNode);
            i = body_end; // Sauter tout le bloc
        }
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
HashTable *variables; // Table de hachage pour stocker les variables
int evaluate_AST(ASTNode *node, HashTable *variables) {
    if (!node) return 0;
    // Ajout pour débogage
    /*printf("Debug: Node type = %d\n", node->type);
    fflush(stdout); // Force l'affichage immédiat*/

    switch (node->type) {
        case TOKEN_NUMBER:
            return atoi(node->value);
        case TOKEN_VARIABLE: {
            int found;
            int value = get_variable(variables, node->value, &found);
            if (!found) {
                printf("Warning: Variable '%s' not found, initializing to 0\n", node->value);
                value = 0;
                set_variable(variables, node->value, value);
            }
            return value;
        }
        case TOKEN_OPERATOR: {
            int left_val = evaluate_AST(node->left, variables);
            int right_val = evaluate_AST(node->right, variables);
            switch (node->value[0]) {
                case '+': return left_val + right_val;
                case '-': return left_val - right_val;
                case '*': return left_val * right_val;
                case '/':
                    if (right_val == 0) {
                        printf("Error: Division by zero\n");
                        return 0;
                    }
                    return left_val / right_val;
                case '<': return left_val < right_val;
                case '>': return left_val > right_val;
                case '=': return left_val == right_val;
                case '!': return left_val != right_val;
                default:
                    printf("Error: Unknown operator '%c'\n", node->value[0]);
                    return 0;
            }
        }
        case TOKEN_ASSIGN: {
            int value = evaluate_AST(node->right, variables);
            set_variable(variables, node->left->value, value);
            return value;
        }
        case TOKEN_WHILE: {
            printf("Évaluation de la condition WHILE : ");
            print_AST(node->left, 0);
            while (evaluate_AST(node->left, variables)) {
                printf("Condition vraie, exécution du corps\n");
                evaluate_AST(node->right, variables);
                print_table(variables); // Vérifie l'état des variables après chaque itération
            }
            printf("Fin de la boucle WHILE\n");
        return 0;
}

        case TOKEN_FOR: {
    // Initialisation
    evaluate_AST(node->left, variables); 

    // Condition
    while (evaluate_AST(node->right, variables)) {
        // Corps de la boucle
        evaluate_AST(node->right->right, variables);

        // Incrément
        evaluate_AST(node->extra, variables);
    }
    return 0;
}

        default:
            printf("Error: Unknown node type\n");
            return 0;
    }
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




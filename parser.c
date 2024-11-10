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

void parse(Token *tokens, int num_tokens) {
    PILE operatorStack;
    initialize(&operatorStack);

    // Parcours des tokens générés par le lexer
    for (int i = 0; i < num_tokens; i++) {
        Token token = tokens[i]; // token est une variable locale pour stocker le tokens[i]

        if (token.type == TOKEN_NUMBER || token.type == TOKEN_VARIABLE) {
            // Ajouter les nombres/variables directement dans la sortie
            printf("%s ", token.value);
        }
        else if (token.type == TOKEN_OPERATOR) {
            // Gérer les priorités et vider les opérateurs de la pile
            while (!isEmpty(&operatorStack) && priority(peek(&operatorStack)) >= priority(token.value[0])) {
                printf("%c ", pop(&operatorStack));
            }
            push(&operatorStack, token.value[0]);
        }
        else if (token.type == TOKEN_PARENTHESIS) {
            if (token.value[0] == '(') {
                push(&operatorStack, token.value[0]);
            } else {
                // Dépile jusqu'à la parenthèse ouvrante
                while (!isEmpty(&operatorStack) && peek(&operatorStack) != '(') { 
                    printf("%c ", pop(&operatorStack)); // le programme va dépiler la pile jusqu'à qu'il rencontre une parenthèse ouvrante 
                }
                pop(&operatorStack); // Retirer '('
            }
        }
    }

    // Déplacer les opérateurs restants de la pile vers la sortie
    while (!isEmpty(&operatorStack)) {
        printf("%c ", pop(&operatorStack));
    }
    printf("\n");
}

// Fonction de priorité des opérateurs
int priority(char operator) {
    if (operator == '+' || operator == '-') return 1;
    if (operator == '*' || operator == '/') return 2;
    return 0;
}



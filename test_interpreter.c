#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"
#include "parser.c"
#include "hash_table.h"
#include "io.c"
#include "io.h"

void test_interpreter(const char *input, HashTable *variables) {
    // Étape 1 : Tokenizer l'entrée
    Token *tokens = lexer(input);
    int num_tokens = get_token_count();

    // Étape 2 : Parser pour construire l'AST
    ASTNode *ast = parse(tokens, num_tokens);

    // Étape 3 : Évaluation de l'AST
    int result = evaluate_AST(ast, variables);
    print_result(result);

    // Libérer la mémoire
    free_AST(ast);
    free_tokens();
}

void read_file_and_execute(const char *filename,HashTable *variables) {
     set_variable(variables,"a",7);
    //Ouvrir le fichier pour lire les instructions
    FILE *file = fopen(filename, "r");
    if(!file) {
        perror("Erreur lors de l'ouverture du fihchier");
        return ;
    }

    // Lire le fichier lignepar ligne
    char line[256]; // taille d'un tableau qui peut contenir jusqu'a 255 caractères
    while(fgets(line, sizeof(line), file)) {
        // Supprimer le saut de ligne
        line[strcspn(line,"\n")] = 0;

        //Afficher la ligne lue
        printf("Ligne lue : %s\n", line);

        //Traiter la ligne avec la fonction d'intérpréteur
        test_interpreter(line, variables);
    }

    // Fermer le fichier après la lecture
    fclose(file);
}

// Fonction pour le mode interactif (REPL)
void interactive_mode(HashTable *variables) {
    char input[256];
    printf("Mode Interactif . Tapez 'exit' pour quitter. \n");

    while(1) {
        printf(">>>"); // Invite pour l'utilisateur
        if(fgets(input, sizeof(input),stdin)) {
            input[strcspn(input, "\n")] = 0; // Supprimer le saut de ligne

            // Si l'utilisateur tape 'exit' , on quitte le mode interactif
            if(strcmp(input,"exit") == 0) {
                printf("Quitter le mode interactif \n");
                break;
            }
            test_interpreter(input,variables);
        }
    }
}

int main() {
    // Création de la table de hachage pour les variables
    HashTable *table = create_table();

    int mode;
    printf("Choisissez un mode \n");
    printf("1. Mode fichier\n");
    printf("2. Mode interactif\n");
    printf(" Votre choix : ");
    scanf("%d", &mode);
    getchar();

    if(mode == 1) {
        //Mode fichier
        read_file_and_execute("C:\\Users\\akric\\OneDrive\\Documents\\test.txt", table);
    } else if (mode == 2) {
        // Mode interactif
        interactive_mode(table);
    } else {
        printf("Choix invalide\n");
    }

/*
   set_variable(table, "a", 7);
    // Ajout d'une variable 'a' dans la table
     // Ajout de la variable 'a' avec la valeur 7
    // Tester l'interpréteur avec une expression (par exemple: "a + 3")
    /*test_interpreter("a + 3", table);  // Test de l'expression 'a + 3'*/
/*
    printf("Test avec l'expression 'b = a + 3':\n");
    test_interpreter("b = a + 3", table);  // Test de l'assignation 'b = a + 3'

    printf("Test avec l'expression '5 + 2':\n");
    test_interpreter("5 + 2", table); // Test d'une simple somme

    printf("Test avec l'expression 'y = x + 5':\n");
    set_variable(table, "x", 10);
    test_interpreter("x + 5", table);

    printf("Test avec l'expression 'a = 3 + (4 * 2)':\n");
    set_variable(table, "a", 3 + 4 * 2 );
    test_interpreter("a", table);  // Doit donner a = 11

    printf("Test avec l'expression 'b = (1 + 2) * (3 - 1)':\n");
    set_variable(table, "b", (1 + 2) * (3 - 1) );
    test_interpreter("b ", table);  // Doit donner b = 6

*/
    // Libérer la table de hachage
    free_table(table);
    }
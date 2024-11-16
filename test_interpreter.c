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
    printf("Résultat de l'expression '%s' : %d\n", input, result);
     // Libérer la mémoire
    free_AST(ast);
    free_tokens();
}
void read_file_and_execute(const char *filename, HashTable *variables) {
    // Ouvrir le fichier pour lire les instructions
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Lire le fichier ligne par ligne
    char line[256]; // Taille d'un tableau qui peut contenir jusqu'à 255 caractères
    while (fgets(line, sizeof(line), file)) {
        // Supprimer le saut de ligne
        line[strcspn(line, "\n")] = 0;

        // Vérifier si la ligne est vide ou contient uniquement des espaces
        if (strlen(line) == 0) {
            continue; // Ligne vide, on l'ignore
        }
        int only_whitespace = 1;
        for (int i = 0; line[i] != '\0'; i++) {
            if (!isspace(line[i])) {
                only_whitespace = 0;
                break;
            }
        }
        if (only_whitespace) {
            continue; // Ligne contenant uniquement des espaces
        }

        // Afficher la ligne lue
        printf("Ligne lue : '%s'\n", line);

        // Traiter la ligne avec la fonction d'interpréteur
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
    HashTable *table = create_table();

    printf("Test avec l'expression 'a = 3':\n");
    test_interpreter("a = 3", table);
    //print_table(table);

    printf("\nTest avec l'expression 'b = a + 3':\n");
    test_interpreter("b = a + 3", table);
    //print_table(table);

    printf("\nTest avec l'expression 'c = (a + b) * 2':\n");
    test_interpreter("c = (a + b) * 2", table);
    //print_table(table);

    printf("\nTest avec l'expression 'd = (c + b) * 2':\n");
    test_interpreter("d = (c + b) * 2", table);
    //print_table(table);

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
    free_table(table);
    return 0;
}
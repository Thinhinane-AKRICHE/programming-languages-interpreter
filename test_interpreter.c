#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"
#include "parser.c"
#include "hash_table.h"
#include "io.c"
#include "io.h"

void test_interpreter(const char *input, HashTable *variables) {
    Token *tokens = lexer(input);
    int num_tokens = get_token_count();
    ASTNode *ast = parse(tokens, num_tokens);
    
    /*printf("AST généré pour l'entrée : '%s'\n", input);
    print_AST
    (ast, 0);  // Affiche l'AST avec un niveau d'indentation de 0*/

    if (ast->type == TOKEN_WHILE || ast->type == TOKEN_FOR) {
        
        evaluate_AST(ast, variables);
        printf("Boucle exécutée\n");
    } else {
        int result = evaluate_AST(ast, variables);
        print_result(result);
        printf("Résultat de l'expression '%s' : %d\n", input, result);
    }

    free_AST(ast);
    free_tokens();
}

void read_file_and_execute(const char *filename, HashTable *variables) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char line[256];  // Déclarez la variable line ici
    char input[1000] = "";
    int brace_count = 0;
    int in_control_structure = 0;

    while (fgets(line, sizeof(line), file)) {
        // ... (traitement de la ligne)
        
        strcat(input, line);
        strcat(input, " ");

        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '{') brace_count++;
            if (line[i] == '}') brace_count--;
        }

        if (strstr(line, "while") || strstr(line, "for") || brace_count > 0) {
            in_control_structure = 1;
        }

        if (!in_control_structure || (in_control_structure && brace_count == 0)) {
            test_interpreter(input, variables);
            input[0] = '\0';
            in_control_structure = 0;
        }
    }
    // ... (code existant)
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
    print_table(table);

    printf("\nTest avec l'expression 'b = a + 3':\n");
    test_interpreter("b = a + 3", table);
    print_table(table);

    printf("\nTest avec l'expression 'c = (a + b) * 2':\n");
    test_interpreter("c = (a + b) * 2", table);
    print_table(table);

    printf("\nTest avec l'expression 'd = (c + b) * 2':\n");
    test_interpreter("d = (c + b) * 2", table);
    print_table(table);

    // Test 1 : Boucle while

    printf("\nTest : Boucle while\n");
    
    test_interpreter("x = 0", table);
    test_interpreter("y = 3", table);
    set_variable(table,"y", 9);
    test_interpreter("while (x < 5) { y = y - 2; x = x + 1 }", table);
    
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
    // Affichez les résultats finaux des variables pour vérifier
    printf("\nTable des variables finales :\n");
    print_table(table);

    free_table(table);
    return 0;
}
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

int main() {
    // Création de la table de hachage pour les variables
    HashTable *table = create_table();
    
    // Ajout d'une variable 'a' dans la table
    set_variable(table, "a", 7);  // Ajout de la variable 'a' avec la valeur 7
    // Tester l'interpréteur avec une expression (par exemple: "a + 3")
    /*test_interpreter("a + 3", table);  // Test de l'expression 'a + 3'*/

    printf("Test avec l'expression 'b = a + 3':\n");
    test_interpreter("b = a + 3", table);  // Test de l'assignation 'b = a + 3'

    printf("Test avec l'expression '5 + 2':\n");
    test_interpreter("5 + 2", table); // Test d'une simple somme 
    
    printf("Test avec l'expression 'y = x + 5':\n");
    set_variable(table, "x", 10); 
    test_interpreter("x + 5", table);
    
    printf("Test avec l'expression 'a = 3 + 4 * 2':\n");
    set_variable(table, "a", 3 + 4 * 2 );
    test_interpreter("a", table);  // Doit donner a = 11

    printf("Test avec l'expression 'b = (1 + 2) * (3 - 1)':\n");
    set_variable(table, "b", (1 + 2) * (3 - 1) );
    test_interpreter("b ", table);  // Doit donner b = 6


     // Libérer la table de hachage
    free_table(table);

    return 0;
}
#include "io.h"
#include <stdio.h>

/*void print_result(int result, HashTable *variables) {
    printf("Resultat : %d\n", result);
    printf("État de la table de hachage :\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *entry = variables->entries[i];
        while (entry) {
            printf("Variable '%s' = %d\n", entry->key, entry->value);
            entry = entry->next;
        }
    }
}*/
void print_result(int result) {
    printf("Resultat : %d\n", result);
}
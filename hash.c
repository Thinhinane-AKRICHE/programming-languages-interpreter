#include "hash_table.h"
#include <stdio.h>

unsigned int hash(const char *key) {
    unsigned int hash_value = 0;
    unsigned int prime = 31;  // Un nombre premier pour la fonction de hachage

    while (*key) {
        hash_value = hash_value * prime + *key; // Calcul du hash
        key++;
    }

    return hash_value % TABLE_SIZE;  // TABLE_SIZE est la taille de votre table de hachage
}

HashTable* create_table() {
    HashTable *table = malloc(sizeof(HashTable));
    if (table) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table->entries[i] = NULL;
        }
    }
    return table;
}
void print_table(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *entry = table->entries[i];
        while (entry) {
            printf("Variable '%s' = %d\n", entry->key, entry->value);
            entry = entry->next;
        }
    }
}
void free_table(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *entry = table->entries[i];
        while (entry) {
            Entry *tmp = entry;
            entry = entry->next;
            free(tmp->key);
            free(tmp);
        }
    }
    free(table);
}

void set_variable(HashTable *table, const char *key, int value) {
    unsigned int index = hash(key);
    Entry *entry = table->entries[index];
    
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    
    Entry *new_entry = malloc(sizeof(Entry));
    if (new_entry == NULL) {
        printf("Erreur d'allocation mémoire pour la nouvelle entrée\n");
        return;
    }
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = table->entries[index];
    table->entries[index] = new_entry;
}
int get_variable(HashTable *table, const char *key, int *found) {
    unsigned int index = hash(key);
    Entry *entry = table->entries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            *found = 1;
            printf("Variable '%s' trouvée avec valeur : %d\n", key, entry->value);
            return entry->value;
        }
        entry = entry->next;
    }
    *found = 0;
    printf("Variable '%s' non trouvée, initialisation à 0\n", key);
    set_variable(table, key, 0);
    return 0;
}


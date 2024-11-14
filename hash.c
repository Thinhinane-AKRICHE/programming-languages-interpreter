#include "hash_table.h"
#include <stdio.h>

static unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key;
        key++;
    }
    return hash % TABLE_SIZE;
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
    
    /*printf("Ajout de la variable : %s avec la valeur %d ", key, value); // Débogage*/

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            /*printf("Variable existante %s mise à jour avec la valeur %d\n", key, value); // Débogage*/
            return;
        }
        entry = entry->next;
    }

    // Si la variable n'existe pas, on l'ajoute
    entry = malloc(sizeof(Entry));
    entry->key = strdup(key);
    entry->value = value;
    entry->next = table->entries[index];
    table->entries[index] = entry;

    /*printf("Nouvelle variable %s ajoutee avec la valeur %d\n", key, value); // Débogage*/
}

int get_variable(HashTable *table, const char *key, int *found) {
    unsigned int index = hash(key);
    Entry *entry = table->entries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            *found = 1;
            /*printf("Found variable '%s' with value: %d\n", key, entry->value);*/
            return entry->value;
        }
        entry = entry->next;
    }
    *found = 0;
    printf("Variable '%s' not found\n", key);
    return 0;
}

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Structure pour stocker une variable avec son nom et sa valeur
typedef struct Entry {
    char *key;
    int value;
    struct Entry *next; // Chaining en cas de collision
} Entry;

// Table de hachage
typedef struct {
    Entry *entries[TABLE_SIZE];
} HashTable;

// Fonctions pour la table de hachage
HashTable* create_table();
void free_table(HashTable *table);
void set_variable(HashTable *table, const char *key, int value);
int get_variable(HashTable *table, const char *key, int *found);

#endif

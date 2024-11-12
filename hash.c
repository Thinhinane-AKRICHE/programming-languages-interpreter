#include "hash_table.h"

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
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    entry = malloc(sizeof(Entry));
    entry->key = strdup(key);
    entry->value = value;
    entry->next = table->entries[index];
    table->entries[index] = entry;
}

int get_variable(HashTable *table, const char *key, int *found) {
    unsigned int index = hash(key);
    Entry *entry = table->entries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            *found = 1;
            return entry->value;
        }
        entry = entry->next;
    }
    *found = 0;
    return 0;
}

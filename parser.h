#ifndef PARSER_H
#define PARSER_H
#define MAX 100 // taille max de la pile

typedef struct {
    int pile[MAX]; // tableau pour stocker les elmts de la pile
    int TOP; // Sommet de la pile
}PILE;

void initialize();
int isEmpty(PILE *pile);
int isFull(PILE *pile);
void push();
int pop(PILE *pile);
int peek(PILE *pile);

#endif //PARSER_H
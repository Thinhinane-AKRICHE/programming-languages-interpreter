#include <stdio.h>  //pour  les fonctions d'entrée/sortie
#include <stdlib.h> // contient des fonctions pour gérer la mémoire
#include <string.h> // offre des fonctions pour les chaines de caractères
#include <ctype.h> // Manipulation des caractères ( si c'est numérique, lettre,..)
#include "lexer.h"

char* my_strndup(const char *s, size_t n) {
  size_t len = strlen(s ); // Utiliser strnlen pour obtenir la longueur de la chaîne
  if (len == 0) return NULL;
  char *copy = malloc(len + 1); // Allouer de la mémoire pour la nouvelle chaîne
  if (copy) {
    memcpy(copy, s, len); // Copier les caractères
    copy[len] = '\0'; // Ajouter le caractère de fin de chaîne
  }
  return copy; // Retourner la nouvelle chaîne
}


Token *tokens = NULL; // un pointeur vers un tableau de Token
int token_count = 0; // Compteur du nombre de tokens ajoutés au tableau

void add_token(TokenType type, const char* value) {
    Token *new_tokens = realloc(tokens, (token_count + 1) * sizeof(Token)); //pour pouvoir y ajouter un nouveau token au bloc de mémoire ( qui peut etre un tableau) qui sera redimentionné a chaque fois
    // (+1) pour augmenter la taille du tableau a chaque fois on ajoute un token
    if (new_tokens == NULL) {
      perror("Erreur d'allocation");
      exit(EXIT_FAILURE);
    }
   tokens = new_tokens;
   tokens[token_count].type = type; // Assigner le type au nouveau token
   tokens[token_count].value = my_strndup(value , strlen(value));
    // prendre la chaine de caractères et on appelle strdup qui alloue une nouvelle mémoire
    // et copie la chaine de caractères dans la nouvelle mémoire
    // et value est un pointeur qui pointe mnt vers la nouvelle mémoire
  printf("Ajout du token: Type=%d, Value=%s\n", type, tokens[token_count].value); // Ajout d'un message de débogage
  token_count++; // Incrémenter le compteur
}

  // Fonction du lexer
void lexer(const char *input) {
  int i = 0; // pour parcourir chaque caractère

  while ( input[i] != '\0' ) {
    // Boucle principale : Tant que le caractère n'est pas le caractère nul
    if (isspace(input[i])) { // Vérifier si le caractère est un espace
      i++; // Si c'est le cas il incrémente i et passe à l'itération suivante
      continue; // ignorer le reste pour recommencer
    }

    // NOMBRES
    if (isdigit(input[i])) { // Vérifier si le caractère est un chiffre, si c'est vrai on a trouvé le début d'un nombre
      int depart = i; // position de départ d'un token dans la chaine pour qu'il soit reconnu et traité par l'interpreteur par la suite
      while (isdigit(input[i])) { // tant que le caractère suivant est un chiffre
        i++; // on avance pour vérifier le prochain
      }
      // Départ est la position du début et i est la position après le dernier chiffre
      char *num = my_strndup(&input[depart], i - depart); // i-depart : longueur du nombre
      // strndup pour créer une nouvelle chaine de caractères qui contient le nombre
      add_token(TOKEN_NUMBER, num); // Ajout du token de type TOKEN_NUMBER avec la valeur num
      free(num); // Libérer la mémoire
      continue;
    }

    // OPÉRATEURS

    if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') { // Vérifier si le caractère actuel est un opérateur
      char opera[2] = {input[i], '\0'}; // création d'une chaine de caractères pour stocker l'opérateur
      // opera[2] prcq en C , les chaines de caractères doivent tjrs se terminer par un caractère nul '\0'
      add_token(TOKEN_OPERATOR, opera); // ajouter un token de type TOKEN_OPERATOR
      i++; // Passer au caractère suivant
      continue;
    }

    // PARENTHESES

    if(input[i] == '(' || input[i] == ')') {
      char parent[2] = {input[i], '\0'}; //Création d'une chaine de caractère
      add_token(TOKEN_PARENTHESIS, parent);
      i++;
      continue;
    }

    //VARIABLES
    if(isalpha(input[i])) { //si le caractère est une lettre
      int depart_2 = i;
      while(isalpha(input[i]) || isdigit(input[i])) {
        i++;
      }
      char *var = my_strndup(&input[depart_2], i - depart_2);
      add_token(TOKEN_VARIABLE, var);
      free(var);
      continue;
    }

    // ASSIGNATION
    if (input[i] == '=') {
      add_token(TOKEN_ASSIGN, "=");
      i++;
      continue;
    }
    i++; // Passer au caractère suivant
  }
    add_token(TOKEN_END, ""); // Indiquer que l'analyse est terminée

}

    // Fonction pour libérer la mémoire allouée

void free_tokens() {
  if(tokens) {
    for (int i = 0; i < token_count; i++) {
      free(tokens[i].value); // Libérer chaque valeur
    }
    free(tokens);
    tokens = NULL;
  }
}


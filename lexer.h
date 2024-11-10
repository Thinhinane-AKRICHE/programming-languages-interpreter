#ifndef LEXER_H
#define LEXER_H


typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PARENTHESIS,
    TOKEN_VARIABLE,
    TOKEN_ASSIGN,
    TOKEN_END
  } TokenType;


// Structure appelée Token pour stocker les infos sur une variable
typedef struct {
    char *value; // stocker la valeur sous forme de chaine de caractères
    TokenType type; // type du token qui est défini par l'enum TokenType
} Token;

extern int token_count;
extern Token *tokens;

void add_token(TokenType type, const char* value);
void lexer(const char* input);
void free_tokens();

#endif //LEXER_H
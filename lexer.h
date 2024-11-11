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

char *my_strndup(const char *s, size_t n);
void add_token(TokenType type, const char* value);
Token *lexer(const char *input);
void free_tokens();
int get_token_count();

#endif //LEXER_H
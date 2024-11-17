#ifndef LEXER_H
#define LEXER_H


typedef enum {
    TOKEN_NUMBER = 0,
    TOKEN_OPERATOR = 1,
    TOKEN_PARENTHESIS = 2,
    TOKEN_VARIABLE = 3,
    TOKEN_ASSIGN = 4,
    TOKEN_END = 5,
    TOKEN_BRACE_OPEN = 6,   // Accolade ouvrante "{"
    TOKEN_BRACE_CLOSE = 7,  // Accolade fermante "}"
    TOKEN_WHILE = 8,
    TOKEN_FOR = 9,
    TOKEN_BLOCK = 10,
    TOKEN_SEMICOLON =11,
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
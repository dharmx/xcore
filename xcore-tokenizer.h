#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct token_t token_t;
struct token_t {
    char* option;
    char* value;
    token_t* next_token;
};

__attribute_nonnull__((1, 2)) void put_token(token_t**, char*, char*);
__attribute_nonnull__((1, 2, 3))
    __attribute__((returns_nonnull)) token_t* tokenize_args(const char*, const char*, const char*);

// vim:filetype=c

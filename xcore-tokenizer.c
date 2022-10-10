#define _XOPEN_SOURCE 500

#include "xcore-tokenizer.h"

void
put_token(token_t** root_ref, char* new_option, char* new_value) {
    if (*root_ref == NULL) {
        token_t* new_token = calloc(sizeof(token_t), 1);
        new_token->option  = new_option;
        new_token->value   = new_value;
        *root_ref          = new_token;
    } else put_token(&(*root_ref)->next_token, new_option, new_value);
}

token_t*
tokenize_args(const char* token_string, const char* delim, const char* option_delim) {
    token_t* token = NULL;
    token_t* main  = NULL;

    char* temp = strtok(strdup(token_string), delim);
    while (temp != NULL) {
        put_token(&token, temp, NULL);
        temp = strtok(NULL, delim);
    }

    while (token != NULL) {
        char* new_option = strtok(token->option, option_delim);
        char* new_value  = strtok(NULL, option_delim);
        put_token(&main, new_option, new_value);
        token = token->next_token;
    }

    return main;
}

// vim:filetype=c

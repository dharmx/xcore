/* tokenizer.c
 *
 * Copyright 2022 dharmx
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#define _XOPEN_SOURCE 500

#include "tokenizer.h"

void
append_arg(arg_t** arg_ref, char* new_value) {
    if (*arg_ref == NULL) {
        arg_t* new_arg    = calloc(sizeof(arg_t), 1);
        new_arg->value    = new_value;
        new_arg->next_arg = NULL;
        *arg_ref          = new_arg;
        return;
    }
    append_arg(&(*arg_ref)->next_arg, new_value);
}

void
append_token(token_t** token_ref, char* new_option, arg_t* new_args, u_long nargs) {
    if (*token_ref == NULL) {
        token_t* new_token = calloc(sizeof(token_t), 1);
        new_token->option  = new_option;
        new_token->args    = new_args;
        new_token->nargs   = nargs;
        *token_ref         = new_token;
    } else append_token(&(*token_ref)->next_token, new_option, new_args, nargs);
}

token_t*
tokenize_args(char* token_string, char* delim, char* option_delim) {
    arg_t* raw_tokens = NULL;
    token_t* tokens   = NULL;

    char* temp = strtok(token_string, delim);
    while (temp != NULL) {
        append_arg(&raw_tokens, temp);
        temp = strtok(NULL, delim);
    }

    while (raw_tokens != NULL) {
        char* new_option    = strtok(raw_tokens->value, option_delim);
        char* new_arg_value = NULL;
        arg_t* new_arg      = NULL;
        u_long arg_count    = 0;
        while ((new_arg_value = strtok(NULL, option_delim)) != NULL) {
            append_arg(&new_arg, new_arg_value);
            ++arg_count;
        }
        append_token(&tokens, new_option, new_arg, arg_count);
        raw_tokens = raw_tokens->next_arg;
    }

    return tokens;
}

// vim:filetype=c

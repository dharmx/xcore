#pragma once

#include "xcore-types.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

__attribute_nonnull__((2)) void append_arg(arg_t**, char*);
__attribute_nonnull__((1, 2)) void append_token(token_t**, char*, arg_t*, u_long);
__attribute_nonnull__((1, 2, 3)) __attribute__((returns_nonnull)) token_t* tokenize_args(char*, char*, char*);

// vim:filetype=c

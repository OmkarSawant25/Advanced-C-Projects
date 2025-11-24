#include <stdio.h>
#include <stdlib.h>
#include "error_handler.h"

#define RED   "\033[1;31m"
#define RESET "\033[0m"

void lexical_error(const char *msg, int line)
{
    fprintf(stderr, RED "Lexical Error (line %d): %s\n" RESET, line, msg);
    exit(EXIT_FAILURE);
}

void lexical_error_char(char c, int line)
{
    fprintf(stderr, RED "Lexical Error (line %d): Invalid symbol '%c'\n" RESET, line, c);
    exit(EXIT_FAILURE);
}

void syntax_error(const char *msg, int line)
{
    fprintf(stderr, RED "Syntax Error (line %d): %s\n" RESET, line, msg);
    exit(EXIT_FAILURE);
}

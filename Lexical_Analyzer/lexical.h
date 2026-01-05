#ifndef LEXICAL_H
#define LEXICAL_H

#include <stdio.h>
#include "types.h"

typedef struct _Lexical
{
    FILE *fptr;
    char *fname;
    char keywords[32][10];
    int keyword_count;
} Lexical;

void keyword(Lexical *lexi);
Status read_and_validation_args(char *argv[], Lexical *lexi);
Status open_file(Lexical *lexi);
Status start_lexical_analysis(Lexical *lexi);

int is_keyword(Lexical *lexi, char *word);
void skip_header_files(Lexical *lexi, int *ch, int *line);
void print_token(char *type, char *lexeme);

#endif

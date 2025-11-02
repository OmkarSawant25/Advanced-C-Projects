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

// Token representation
typedef struct
{
    char lexeme[50];
    char type[20];
} Token;

void keyword(Lexical *lexi);
Status read_and_validation_args(char *argv[], Lexical *lexi);
Status open_file(Lexical *lexi);
Status start_lexical_analysis(Lexical *lexi);

#endif

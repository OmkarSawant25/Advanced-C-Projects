#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexical.h"
#include "types.h"

void skip_header_files(Lexical *lexi, char *ch)
{
    while (*ch != '\n' && *ch != EOF)
    {
        *ch = fgetc(lexi->fptr);
    }

    *ch = fgetc(lexi->fptr);
}

int is_keyword(Lexical *lexi, char *word)
{
    for (int i = 0; i < lexi->keyword_count; i++)
    {
        if (strcmp(word, lexi->keywords[i]) == 0)
            return 1;
    }
    return 0;
}

Status start_lexical_analysis(Lexical *lexi)
{
    char ch, next;
    char buffer[100];
    int i;

    printf("TOKENS FOUND:\n");
    printf("-------------------------------------------------\n");

    ch = fgetc(lexi->fptr);

    while (ch != EOF)
    {
        if (ch == '#')
        {
            skip_header_files(lexi, &ch);
            continue;
        }
        if (isspace(ch))
        {
            ch = fgetc(lexi->fptr);
            continue;
        }

        // Identifier or Keyword
        if (isalpha(ch) || ch == '_')
        {
            i = 0;
            while (isalpha(ch) || isdigit(ch) || ch == '_')
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }
            buffer[i] = '\0';

            if (is_keyword(lexi, buffer))
                printf("Keyword\t\t\t:\t%s\n", buffer);
            else
                printf("Identifier\t\t:\t%s\n", buffer);
            continue;
        }

        // Numeric Constant
        else if (isdigit(ch))
        {
            i = 0;
            int has_dot = 0;
            int is_hex = 0;
            int is_octal = 0;
            int is_binary = 0;

            // Check for Hexadecimal or Octal
            if (ch == '0')
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);

                if (ch == 'x' || ch == 'X')
                {
                    is_hex = 1;
                    buffer[i++] = ch;
                    ch = fgetc(lexi->fptr);

                    while (isxdigit(ch))
                    {
                        buffer[i++] = ch;
                        ch = fgetc(lexi->fptr);
                    }
                    buffer[i] = '\0';
                    printf("Numeric constant\t:\t%s\n", buffer);
                    continue;
                }
                else if (ch == 'b')
                {
                    is_binary = 1;
                    buffer[i++] = ch;
                    ch = fgetc(lexi->fptr);

                    while (ch == '0' || ch == '1')
                    {
                        buffer[i++] = ch;
                        ch = fgetc(lexi->fptr);
                    }
                    buffer[i] = '\0';
                    printf("Numeric constant\t:\t%s\n", buffer);
                    continue;
                }
                else if (isdigit(ch))
                {
                    is_octal = 1;
                    while (ch >= '0' && ch <= '7')
                    {
                        buffer[i++] = ch;
                        ch = fgetc(lexi->fptr);
                    }
                    buffer[i] = '\0';
                    printf("Numeric constant\t:\t%s\n", buffer);
                    continue;
                }
                // If just '0' alone
                else
                {
                    buffer[i] = '\0';
                    printf("Numeric constant\t:\t%s\n", buffer);
                    continue;
                }
            }

            while (isdigit(ch) || ch == '.')
            {
                if (ch == '.')
                {
                    if (has_dot)
                        break;
                    has_dot = 1;
                }
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }
            buffer[i] = '\0';

            if (has_dot)
                printf("Float constant\t\t:\t%s\n", buffer);
            else
                printf("Numeric constant\t:\t%s\n", buffer);
            continue;
        }

        // String Literal
        else if (ch == '\"')
        {
            i = 0;
            buffer[i++] = ch;
            ch = fgetc(lexi->fptr);
            while (ch != EOF && ch != '\"')
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }
            if (ch == '\"')
            {
                buffer[i++] = '\"';
                buffer[i] = '\0';
                printf("String literal\t\t:\t%s\n", buffer);
                ch = fgetc(lexi->fptr);
            }
            continue;
        }

        // Character Constant
        else if (ch == '\'')
        {
            i = 0;
            buffer[i++] = ch;
            ch = fgetc(lexi->fptr);

            if (ch == '\\')
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }
            else if (ch != EOF)
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }

            if (ch == '\'')
            {
                buffer[i++] = '\'';
                buffer[i] = '\0';
                printf("Character const\t\t:\t%s\n", buffer);
                ch = fgetc(lexi->fptr);
            }
            continue;
        }

        // Comments or Division
        else if (ch == '/')
        {
            next = fgetc(lexi->fptr);
            if (next == '/')
            {
                while ((ch = fgetc(lexi->fptr)) != EOF && ch != '\n')
                    ;
                ch = fgetc(lexi->fptr);
                continue;
            }
            else if (next == '*')
            {
                char prev = 0;
                while ((ch = fgetc(lexi->fptr)) != EOF)
                {
                    if (prev == '*' && ch == '/')
                        break;
                    prev = ch;
                }
                ch = fgetc(lexi->fptr);
                continue;
            }
            else
            {
                printf("Operator\t\t:\t/\n");
                ch = next;
                continue;
            }
        }

        // Operators
        else if (strchr("+-*=<>!&|%^", ch))
        {
            next = fgetc(lexi->fptr);
            if (next == '=')
            {
                printf("Operator\t\t:\t%c%c\n", ch, next);
                ch = fgetc(lexi->fptr);
            }
            else if ((ch == '+' && next == '+') || (ch == '-' && next == '-') || (ch == '&' && next == '&') || (ch == '|' && next == '|'))
            {
                printf("Operator\t\t:\t%c%c\n", ch, next);
                ch = fgetc(lexi->fptr);
            }
            else
            {
                printf("Operator\t\t:\t%c\n", ch);
                ch = next;
            }
            continue;
        }

        // Symbols
        else if (strchr(";,{}()[]#", ch))
        {
            printf("Symbol\t\t\t:\t%c\n", ch);
            ch = fgetc(lexi->fptr);
            continue;
        }

        // Skip unknowns
        else
        {
            ch = fgetc(lexi->fptr);
        }
    }

    printf("-------------------------------------------------\n");
    return e_success;
}

void keyword(Lexical *lexi)
{
    const char *temp_keywords[32] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"};

    for (int i = 0; i < 32; i++)
        strcpy(lexi->keywords[i], temp_keywords[i]);

    lexi->keyword_count = 32;
}

Status read_and_validation_args(char *argv[], Lexical *lexi)
{
    if (argv[1][0] == '.')
    {
        fprintf(stderr, "Error: Invalid file name '%s'\n", argv[1]);
        return e_failure;
    }

    char *dot = strrchr(argv[1], '.');

    if (dot == NULL)
    {
        fprintf(stderr, "Error: File '%s' has no extension.\n", argv[1]);
        return e_failure;
    }

    if (strcmp(dot, ".c") == 0)
    {
        lexi->fname = argv[1];
        return e_success;
    }

    fprintf(stderr, "Error: Invalid file type '%s' (expected .c)\n", argv[1]);
    return e_failure;
}

Status open_file(Lexical *lexi)
{
    lexi->fptr = fopen(lexi->fname, "r");
    if (lexi->fptr == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", lexi->fname);
        return e_failure;
    }
    return e_success;
}

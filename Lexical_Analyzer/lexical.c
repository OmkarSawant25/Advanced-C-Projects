#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexical.h"
#include "types.h"

#define BUF_SIZE 256   // Buffer size for tokens

/* SIMPLE COLORS */
#define RESET   "\033[0m"
#define YELLOW  "\033[1;33m"
#define KEYWORD_COLOR     "\033[1;34m"  // Keywords
#define IDENTIFIER_COLOR  "\033[0;36m"  // Identifiers
#define NUMBER_COLOR      "\033[0;32m"  // Numbers
#define STRING_COLOR      "\033[0;35m"  // Strings
#define CHAR_COLOR        "\033[0;33m"  // Characters
#define OPERATOR_COLOR    "\033[1;31m"  // Operators
#define SYMBOL_COLOR      "\033[1;37m"  // Symbols
#define DEFAULT_COLOR     "\033[0m"

/* Print token with color */
void print_token(const char *type, const char *lexeme)
{
    const char *color = DEFAULT_COLOR;

    if (strcmp(type, "Keyword") == 0)
        color = KEYWORD_COLOR;
    else if (strcmp(type, "Identifier") == 0)
        color = IDENTIFIER_COLOR;
    else if (strcmp(type, "Numeric constant") == 0 ||
             strcmp(type, "Float constant") == 0)
        color = NUMBER_COLOR;
    else if (strcmp(type, "String literal") == 0)
        color = STRING_COLOR;
    else if (strcmp(type, "Character const") == 0)
        color = CHAR_COLOR;
    else if (strcmp(type, "Operator") == 0)
        color = OPERATOR_COLOR;
    else if (strcmp(type, "Symbol") == 0)
        color = SYMBOL_COLOR;

    printf("%s%-20s : %s%s%s\n", color, type, color, lexeme, RESET);
}

/* Skip preprocessor directives */
void skip_header_files(Lexical *lexi, int *ch, int *line)
{
    while (*ch != '\n' && *ch != EOF)
        *ch = fgetc(lexi->fptr);

    if (*ch == '\n')
        (*line)++;

    *ch = fgetc(lexi->fptr);
}

/* Check for keyword */
int is_keyword(Lexical *lexi, char *word)
{
    for (int i = 0; i < lexi->keyword_count; i++)
    {
        if (strcmp(word, lexi->keywords[i]) == 0)
            return 1;
    }
    return 0;
}

/* Start lexical analysis */
Status start_lexical_analysis(Lexical *lexi)
{
    int ch, next;
    char buffer[BUF_SIZE];
    int i;
    int line = 1;

    printf(YELLOW "TOKENS FOUND:\n" RESET);
    printf("--------------------------------------------\n");

    ch = fgetc(lexi->fptr);

    while (ch != EOF)
    {
        if (ch == '\n')   // New line
        {
            line++;
            ch = fgetc(lexi->fptr);
            continue;
        }

        if (isspace(ch)) // Skip spaces
        {
            ch = fgetc(lexi->fptr);
            continue;
        }

        /* Preprocessor */
        if (ch == '#')
        {
            skip_header_files(lexi, &ch, &line);
            continue;
        }

        /* Identifier or keyword */
        if (isalpha(ch) || ch == '_')
        {
            i = 0;
            while (isalnum(ch) || ch == '_')
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }
            buffer[i] = '\0';

            if (is_keyword(lexi, buffer))
                print_token("Keyword", buffer);
            else
                print_token("Identifier", buffer);

            continue;
        }

        /* Numeric or float constant */
        if (isdigit(ch))
        {
            i = 0;
            int has_dot = 0;

            while (isdigit(ch) || ch == '.')
            {
                if (ch == '.' && has_dot)
                    break;
                if (ch == '.')
                    has_dot = 1;

                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }
            buffer[i] = '\0';

            if (has_dot)
                print_token("Float constant", buffer);
            else
                print_token("Numeric constant", buffer);

            continue;
        }

        /* String literal */
        if (ch == '"')
        {
            i = 0;
            buffer[i++] = ch;
            ch = fgetc(lexi->fptr);

            while (ch != '"' && ch != '\n' && ch != EOF)
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }

            buffer[i++] = '"';
            buffer[i] = '\0';

            print_token("String literal", buffer);
            ch = fgetc(lexi->fptr);
            continue;
        }

        /* Character constant */
        if (ch == '\'')
        {
            i = 0;
            buffer[i++] = ch;
            ch = fgetc(lexi->fptr);

            if (ch == '\\')   // Escape sequence
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }
            else
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }

            buffer[i++] = '\'';
            buffer[i] = '\0';

            print_token("Character const", buffer);
            ch = fgetc(lexi->fptr);
            continue;
        }

        /* Comments or division */
        if (ch == '/')
        {
            next = fgetc(lexi->fptr);

            if (next == '/')   // Single-line comment
            {
                while ((ch = fgetc(lexi->fptr)) != '\n' && ch != EOF);
                if (ch == '\n')
                    line++;
                ch = fgetc(lexi->fptr);
                continue;
            }

            if (next == '*')   // Multi-line comment
            {
                int prev = 0;
                while ((ch = fgetc(lexi->fptr)) != EOF)
                {
                    if (ch == '\n')
                        line++;
                    if (prev == '*' && ch == '/')
                        break;
                    prev = ch;
                }
                ch = fgetc(lexi->fptr);
                continue;
            }

            print_token("Operator", "/"); // Division operator
            ch = next;
            continue;
        }

        /* Operators */
        if (strchr("+-*=<>!&|%^", ch))
        {
            next = fgetc(lexi->fptr);

            if (next == '=' ||
                (ch == '+' && next == '+') ||
                (ch == '-' && next == '-') ||
                (ch == '&' && next == '&') ||
                (ch == '|' && next == '|'))
            {
                char op[3] = {ch, next, '\0'};
                print_token("Operator", op);
                ch = fgetc(lexi->fptr);
            }
            else
            {
                char op[2] = {ch, '\0'};
                print_token("Operator", op);
                ch = next;
            }
            continue;
        }

        /* Symbols */
        if (strchr(";,{}()[]#", ch))
        {
            char sym[2] = {ch, '\0'};
            print_token("Symbol", sym);
            ch = fgetc(lexi->fptr);
            continue;
        }

        /* Unknown character */
        ch = fgetc(lexi->fptr);
    }

    printf("--------------------------------------------\n");

    return e_success;
}

/* Load C keywords */
void keyword(Lexical *lexi)
{
    const char *kw[] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"};

    for (int i = 0; i < 32; i++)
        strcpy(lexi->keywords[i], kw[i]);

    lexi->keyword_count = 32;
}

/* Validate input file */
Status read_and_validation_args(char *argv[], Lexical *lexi)
{
    if (argv[1][0] == '.') // Hidden file check
    {
        fprintf(stderr, "Error: Invalid file name '%s'\n", argv[1]);
        return e_failure;
    }

    char *dot = strrchr(argv[1], '.');
    if (!dot)
    {
        fprintf(stderr, "Error: File '%s' has no extension.\n", argv[1]);
        return e_failure;
    }

    if (strcmp(dot, ".c") == 0)
    {
        lexi->fname = argv[1]; // Store file name
        return e_success;
    }

    fprintf(stderr, "Error: Invalid file type '%s' (expected .c)\n", argv[1]);
    return e_failure;
}

/* Open source file */
Status open_file(Lexical *lexi)
{
    lexi->fptr = fopen(lexi->fname, "r"); // Open C file
    if (!lexi->fptr)
    {
        perror("fopen");
        return e_failure;
    }
    return e_success;
}

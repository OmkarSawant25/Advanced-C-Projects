#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexical.h"
#include "types.h"
#include "error_handler.h"

#define BUF_SIZE 256

#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

/* ===== NEON COLORS (256-COLOR ATTRACTIVE THEME) ===== */
#define NEON_KEYWORD "\033[1;38;5;39m"    // Electric Cyan
#define NEON_IDENTIFIER "\033[1;38;5;45m" // Neon Aqua
#define NEON_NUMERIC "\033[1;38;5;82m"    // Bright Neon Green
#define NEON_FLOAT "\033[1;38;5;118m"     // Light Neon Green
#define NEON_STRING "\033[1;38;5;207m"    // Hot Pink Neon
#define NEON_CHAR "\033[1;38;5;171m"      // Purple-Pink Neon
#define NEON_OPERATOR "\033[1;38;5;226m"  // Neon Yellow
#define NEON_SYMBOL "\033[1;38;5;208m"    // Neon Orange
#define NEON_WHITE "\033[1;37m"

/* Print with COLORS */
static void print_token(const char *type, const char *lexeme)
{
    const char *color;

    if (strcmp(type, "Keyword") == 0)
        color = NEON_KEYWORD;
    else if (strcmp(type, "Identifier") == 0)
        color = NEON_IDENTIFIER;
    else if (strcmp(type, "Numeric constant") == 0)
        color = NEON_NUMERIC;
    else if (strcmp(type, "Float constant") == 0)
        color = NEON_FLOAT;
    else if (strcmp(type, "String literal") == 0)
        color = NEON_STRING;
    else if (strcmp(type, "Character const") == 0)
        color = NEON_CHAR;
    else if (strcmp(type, "Operator") == 0)
        color = NEON_OPERATOR;
    else if (strcmp(type, "Symbol") == 0)
        color = NEON_SYMBOL;
    else
        color = NEON_WHITE;

    printf("%s%-20s : %s%s%s\n", color, type, color, lexeme, RESET);
}

/* UPDATED: skip header, update line */
void skip_header_files(Lexical *lexi, int *ch, int *line)
{
    while (*ch != '\n' && *ch != EOF)
        *ch = fgetc(lexi->fptr);

    if (*ch == '\n')
        (*line)++;

    *ch = fgetc(lexi->fptr);
}

/* Keyword check */
int is_keyword(Lexical *lexi, char *word)
{
    for (int i = 0; i < lexi->keyword_count; i++)
    {
        if (strcmp(word, lexi->keywords[i]) == 0)
            return 1;
    }
    return 0;
}

/* MAIN LEXICAL ANALYZER */
Status start_lexical_analysis(Lexical *lexi)
{
    int ch, next;
    char buffer[BUF_SIZE];
    int i;
    int line = 1;

    printf(YELLOW "TOKENS FOUND:\n" RESET);
    printf(YELLOW "-------------------------------------------------\n" RESET);

    ch = fgetc(lexi->fptr);

    while (ch != EOF)
    {
        if (ch == '\n')
        {
            line++;
            ch = fgetc(lexi->fptr);
            continue;
        }

        if (isspace(ch))
        {
            ch = fgetc(lexi->fptr);
            continue;
        }

        /* PREPROCESSOR */
        if (ch == '#')
        {
            skip_header_files(lexi, &ch, &line);
            continue;
        }

        /* IDENTIFIER OR KEYWORD */
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

        /* NUMERIC CONSTANT */
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

        /* STRING LITERAL */
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

            if (ch == '\n' || ch == EOF)
                lexical_error("Unterminated string literal", line);

            buffer[i++] = '"';
            buffer[i] = '\0';

            print_token("String literal", buffer);
            ch = fgetc(lexi->fptr);
            continue;
        }

        /* CHARACTER LITERAL */
        if (ch == '\'')
        {
            i = 0;
            buffer[i++] = ch;
            ch = fgetc(lexi->fptr);

            if (ch == '\n' || ch == EOF)
                lexical_error("Unterminated character literal", line);

            if (ch == '\\')
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);

                if (ch == '\n' || ch == EOF)
                    lexical_error("Invalid escape sequence", line);

                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }
            else
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }

            if (ch != '\'')
                lexical_error("Unterminated character literal", line);

            buffer[i++] = '\'';
            buffer[i] = '\0';

            print_token("Character const", buffer);

            ch = fgetc(lexi->fptr);
            continue;
        }

        /* COMMENTS OR DIVISION */
        if (ch == '/')
        {
            next = fgetc(lexi->fptr);

            if (next == '/')
            {
                while ((ch = fgetc(lexi->fptr)) != '\n' && ch != EOF)
                    ;
                if (ch == '\n')
                    line++;
                ch = fgetc(lexi->fptr);
                continue;
            }

            if (next == '*')
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

            print_token("Operator", "/");
            ch = next;
            continue;
        }

        /* OPERATORS */
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

        /* SYMBOLS */
        if (strchr(";,{}()[]#", ch))
        {
            char sym[2] = {ch, '\0'};
            print_token("Symbol", sym);
            ch = fgetc(lexi->fptr);
            continue;
        }

        /* UNKNOWN CHARACTER */
        lexical_error_char(ch, line);
    }

    printf(YELLOW "-------------------------------------------------\n" RESET);
    return e_success;
}

/* ADD KEYWORDS */
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

/* MISSING FUNCTIONS ADDED BACK */

Status read_and_validation_args(char *argv[], Lexical *lexi)
{
    if (argv[1][0] == '.')
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
        lexi->fname = argv[1];
        return e_success;
    }

    fprintf(stderr, "Error: Invalid file type '%s' (expected .c)\n", argv[1]);
    return e_failure;
}

Status open_file(Lexical *lexi)
{
    lexi->fptr = fopen(lexi->fname, "r");
    if (!lexi->fptr)
    {
        perror("fopen");
        return e_failure;
    }
    return e_success;
}

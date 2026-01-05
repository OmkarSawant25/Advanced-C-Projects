#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexical.h"
#include "types.h"

#define BUF_SIZE 256 // Buffer size for tokens

void print_token(char *type, char *lexeme)
{
    printf("%-20s :\t%s\n", type, lexeme);
}

void print_error_token(const char *msg, int line)
{
    printf("\n%s %d\n", msg, line);
}

// Skip preprocessor directives
void skip_header_files(Lexical *lexi, int *ch, int *line)
{
    while (*ch != '\n' && *ch != EOF)
        *ch = fgetc(lexi->fptr);

    if (*ch == '\n')
        (*line)++;

    *ch = fgetc(lexi->fptr);
}

// Check for keyword
int is_keyword(Lexical *lexi, char *word)
{
    for (int i = 0; i < lexi->keyword_count; i++)
    {
        if (strcmp(word, lexi->keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Start lexical analysis
Status start_lexical_analysis(Lexical *lexi)
{
    int ch, next;
    char buffer[BUF_SIZE];
    int i;
    int line = 1;
    int main_seen = 0;
    int paren_count = 0;
    int brace_count = 0;
    int main_stage = 0;

    printf("TOKENS FOUND:\n");
    printf("--------------------------------------------\n");

    ch = fgetc(lexi->fptr);

    while (ch != EOF)
    {
        if (ch == '\n') // New line
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

        // Preprocessor
        if (ch == '#')
        {
            skip_header_files(lexi, &ch, &line);
            continue;
        }

        // Identifier or keyword
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
            {
                print_token("Identifier", buffer);

                if (strcmp(buffer, "main") == 0)
                {
                    main_seen = 1;
                    main_stage = 1;
                }
            }

            continue;
        }

        // Numeric constants (Decimal / Float / Hex / Octal / Binary)
        if (isdigit(ch))
        {
            i = 0;
            buffer[i++] = ch;
            ch = fgetc(lexi->fptr);

            // Error: digit followed by alphabet (invalid identifier)
            if ((isalpha(ch) || ch == '_') && !(buffer[0] == '0' && (ch == 'x' || ch == 'X' || ch == 'b' || ch == 'B')))
            {
                print_error_token("Error: Invalid identifier starting with digit on line", line);
                return e_failure;
            }

            // Case: starts with 0 (hex / binary / octal)
            if (buffer[0] == '0')
            {
                // Hexadecimal
                if (ch == 'x' || ch == 'X')
                {
                    buffer[i++] = ch;
                    ch = fgetc(lexi->fptr);

                    int has_hex_digit = 0;

                    while (isalnum(ch))
                    {
                        if (!isxdigit(ch))
                        {
                            print_error_token(
                                "Error: Invalid Hexadecimal Number on line",
                                line);
                            return e_failure;
                        }

                        has_hex_digit = 1;
                        buffer[i++] = ch;
                        ch = fgetc(lexi->fptr);
                    }

                    if (!has_hex_digit)
                    {
                        print_error_token(
                            "Error: Invalid Hexadecimal Number on line",
                            line);
                        return e_failure;
                    }

                    buffer[i] = '\0';
                    print_token("Numeric constant", buffer);
                    continue;
                }

                // Binary
                if (ch == 'b' || ch == 'B')
                {
                    buffer[i++] = ch;
                    ch = fgetc(lexi->fptr);

                    int has_bit = 0;

                    while (isalnum(ch))
                    {
                        if (ch != '0' && ch != '1')
                        {
                            print_error_token(
                                "Error: Invalid Binary Number on line",
                                line);
                            return e_failure; // stop on first error
                        }

                        has_bit = 1;
                        buffer[i++] = ch;
                        ch = fgetc(lexi->fptr);
                    }

                    if (!has_bit)
                    {
                        print_error_token(
                            "Error: Invalid Binary Number on line",
                            line);
                        return e_failure;
                    }

                    buffer[i] = '\0';
                    print_token("Numeric constant", buffer);
                    continue;
                }

                // Octal: starts with 0 followed by digits 0–7
                int octal_error = 0;

                while (isalnum(ch))
                {
                    if (ch >= '0' && ch <= '7')
                    {
                        buffer[i++] = ch;
                    }
                    else
                    {
                        octal_error = 1;
                        break;
                    }
                    ch = fgetc(lexi->fptr);
                }

                if (octal_error)
                {
                    print_error_token(
                        "Error: Invalid Octal Number on line",
                        line);
                    return e_failure;
                }

                buffer[i] = '\0';
                print_token("Numeric constant", buffer);
                continue;
            }

            // Decimal / Float
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

        // String literal
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

            // Error: string not closed
            if (ch != '"')
            {
                print_error_token("Error: Unterminated string literal on line", line);
                return e_failure;
            }

            buffer[i++] = '"';
            buffer[i] = '\0';

            print_token("String literal", buffer);
            ch = fgetc(lexi->fptr);
            continue;
        }

        // Character constant
        if (ch == '\'')
        {
            i = 0;
            buffer[i++] = ch;
            ch = fgetc(lexi->fptr);

            // Error: EOF or newline immediately after '
            if (ch == '\n' || ch == EOF)
            {
                print_error_token(
                    "Error: Unterminated character constant on line",
                    line);
                return e_failure;
            }

            // Error: empty character constant ''
            if (ch == '\'')
            {
                print_error_token(
                    "Error: Empty character constant on line",
                    line);
                return e_failure;
            }

            // Escape sequence
            if (ch == '\\')
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);

                if (ch == '\n' || ch == EOF)
                {
                    print_error_token(
                        "Error: Unterminated character constant on line",
                        line);
                    return e_failure;
                }

                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }
            else
            {
                buffer[i++] = ch;
                ch = fgetc(lexi->fptr);
            }

            // Unterminated character constant
            if (ch == '\n' || ch == EOF || ch == ';')
            {
                print_error_token(
                    "Error: Unterminated character constant on line",
                    line);
                return e_failure;
            }

            // Too many characters like 'ab'
            if (ch != '\'')
            {
                print_error_token(
                    "Error: Too many characters in character constant on line",
                    line);
                return e_failure;
            }

            buffer[i++] = '\'';
            buffer[i] = '\0';

            print_token("Character const", buffer);
            ch = fgetc(lexi->fptr);
            continue;
        }

        // Comments or division
        if (ch == '/')
        {
            next = fgetc(lexi->fptr);

            if (next == '/') // Single-line comment
            {
                while ((ch = fgetc(lexi->fptr)) != '\n' && ch != EOF)
                    ;
                if (ch == '\n')
                    line++;
                ch = fgetc(lexi->fptr);
                continue;
            }

            if (next == '*') // Multi-line comment
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

        // Operators
        if (strchr("+-*=<>!&|%^", ch))
        {
            next = fgetc(lexi->fptr);

            if (next == '=' || (ch == '+' && next == '+') || (ch == '-' && next == '-') || (ch == '&' && next == '&') || (ch == '|' && next == '|'))
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

        // Symbol
        if (strchr(";,{}()[]#", ch))
        {
            char sym[2] = {ch, '\0'};
            print_token("Symbol", sym);

            if (main_seen)
            {
                if (main_stage == 1) // expecting '('
                {
                    if (ch == '(')
                    {
                        paren_count = 1;
                        main_stage = 2;
                    }
                    else
                    {
                        print_error_token(
                            "Error: Expected '(' after main on line",
                            line);
                        return e_failure;
                    }
                }
                else if (main_stage == 2) // expecting ')'
                {
                    if (ch == ')')
                    {
                        paren_count--;
                        main_stage = 3;
                    }
                    else
                    {
                        print_error_token(
                            "Error: Expected ')' after '(' in main on line",
                            line);
                        return e_failure;
                    }
                }
                else if (main_stage == 3) // expecting '{'
                {
                    if (ch == '{')
                    {
                        main_seen = 0;
                        main_stage = 0;
                    }
                    else
                    {
                        print_error_token(
                            "Error: Expected '{' after main() on line",
                            line);
                        return e_failure;
                    }
                }
            }
            // Count braces after main has started
            if (brace_count >= 0)
            {
                if (ch == '{')
                    brace_count++;
                else if (ch == '}')
                    brace_count--;
            }

            ch = fgetc(lexi->fptr);
            continue;
        }

        // Unknown character
        ch = fgetc(lexi->fptr);
    }

    if (paren_count != 0)
    {
        print_error_token(
            "Error: '(' not closed in main",
            line);
        return e_failure;
    }

    if (brace_count != 0)
    {
        print_error_token(
            "Error: Opening '{' not closed in main",
            line);
        return e_failure;
    }
    if (main_stage == 3)
    {
        print_error_token(
            "Error: Missing '{' after main()",
            line);
        return e_failure;
    }

    printf("--------------------------------------------\n");
    return e_success;
}

// Load C keywords
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

// Validate input file
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

// Open source file
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

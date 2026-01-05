// // #include"main.h"
// int main(int argc, char * argv[])
// {
//     Lexical lexi;
//     lexi.keyword_count = 32;
//     if(argc != 2)
//     {
//         printf("Error! Invalide number of argument passed.\n");
//         return failure;
//     }
//     if(read_and_validation(argv, &lexi) == failure)
//     {
//         printf("Error! Read and Validation failed\n");
//         return failure;
//     }
//     /* Assign keywords to a 2d array */
//     keyword(&lexi);
//     if(start_analysis(&lexi) == failure)
//     {
//         printf("Error! Lexical Analysis failed\n");
//         return failure;
//     }
//     else
//         return success;
// }
// Status open_file(Lexical *lexi)
// {
//     lexi->fptr = fopen(lexi->fname, "r");
//     if(lexi->fptr == NULL)
//     {
//         printf("Error! File not found\n");
//         return failure;
//     }
//     return success;
// }
// Status read_and_validation(char * argv[], Lexical * lexi)
// {
//     char * extn = strrchr(argv[1], '.');
//     if(strcmp(extn, ".c") != 0)
//         return failure;
//     lexi->fname = argv[1];
//     return success;
// }
// void keyword(Lexical *lexi)
// {
//     char key_words[32][10] = {"auto", "break", "case", "char", "const", "continue", "default", "do",
//                                 "double", "else", "enum", "extern", "float", "for", "goto", "if",
//                                 "int", "long", "register", "return", "short", "signed", "sizeof", "static",
//                                 "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
//     for (int i=0; i<lexi->keyword_count; i++)
//         strcpy(lexi->keywords[i], key_words[i]);
// }
// Status check_keyword_identifier(Lexical *lexi)
// {
//     for(int i=0; i<lexi->keyword_count; i++)
//     {
//         if(strcmp(lexi->keywords[i], lexi->buffer) == 0)
//             return is_keyword;
//     }
//     return is_identifier;
// }
// Status start_analysis(Lexical *lexi)
// {
//     char ch, single_quote = '\'', double_quote = '\"';
//     int i, j=0;
//     int brace_count = 0;
//     int parentheses_count = 0;
//     if((open_file(lexi)) != success)
//         return failure;

//     while ((ch = fgetc(lexi->fptr)) != EOF)
//     {
//         // Skip header
//         if (ch == '#')
//         {
//             while (ch != '\n' && ch != EOF)
//             {
//                 ch = fgetc(lexi->fptr);
//             }
//             continue; // Skip to the next line
//         }
//         // if keyword or identifier
//         if(isalpha(ch))
//         {
//             lexi->buffer[j++] = ch;
//             while(isalnum(ch = fgetc(lexi->fptr)))
//             {
//                 lexi->buffer[j++] = ch;
//             }
//             lexi->buffer[j] = '\0';
//             j=0;
//             fseek(lexi->fptr, -1, SEEK_CUR); // Backtrack one character
//             if(check_keyword_identifier(lexi) == is_keyword)
//                 printf("Keyword\t\t:\t%s\n",lexi->buffer);
//             else
//                 printf("Identifier\t:\t%s\n",lexi->buffer);
//         }
//         //if digit
//         else if(isdigit(ch))
//         {
//            // Check for hexadecimal (0x), octal (0), or binary (0b)
//            if(ch == '0')
//            {
//                 lexi->buffer[j++] = ch;
//                 char second_ch = fgetc(lexi->fptr);
//                 if(second_ch == 'x' || second_ch == 'X')        // Hexadecimal number
//                 {
//                     lexi->buffer[j++] = second_ch;
//                     ch = fgetc(lexi->fptr);
//                     if(isxdigit(ch))
//                     {
//                         lexi->buffer[j++] = ch;
//                         while (isxdigit(ch = fgetc(lexi->fptr)))
//                             lexi->buffer[j++] = ch;
//                         lexi->buffer[j++] = '\0';
//                         if(ch != ' ' && ch != ',' && ch != ';')
//                         {
//                             printf("Numeric const\t:\tError: Invalid Hexadecimal Number\n");
//                             return failure;
//                         }
//                         else
//                         {
//                             printf("Numeric const\t:\t%s\n",lexi->buffer);
//                         }
//                     }
//                 }
//                 else if(second_ch == 'b' || second_ch == 'B')       // Binary nubers
//                 {
//                     lexi->buffer[j++] = second_ch;
//                     ch = fgetc(lexi->fptr);
//                     if(ch == '0' || ch == '1')
//                     {
//                         lexi->buffer[j++] = ch;
//                         ch = fgetc(lexi->fptr);
//                         lexi->buffer[j++] = ch;
//                         while (ch == '0' || ch == '1')
//                         {
//                             ch = fgetc(lexi->fptr);
//                             lexi->buffer[j++] = ch;
//                         }
//                         lexi->buffer[j++] = '\0';
//                         if(ch != ' ' && ch != ',' && ch != ';')
//                         {
//                             printf("Numeric const\t:\tError: Invalid Binary Number\n");
//                             return failure;
//                         }
//                         else
//                         {
//                             printf("Numeric const\t:\t%s\n",lexi->buffer);
//                         }
//                     }
//                 }
//                 else        // Octal numbers
//                 {
//                     fseek(lexi->fptr, -1, SEEK_CUR);
//                     while (isdigit(ch = fgetc(lexi->fptr)))
//                     {
//                         if(ch >= '8')
//                         {
//                             printf("Numeric const\t:\tError: Invalid Octal Number\n");
//                             return failure;
//                         }
//                         lexi->buffer[j++] = ch;
//                     }
//                     lexi->buffer[j++] = '\0';
//                     printf("Numeric const\t:\t%s\n",lexi->buffer);
//                 }
//            }
//            else
//            {
//                 int is_float = 0;
//                 lexi->buffer[j++] = ch;
//                 while(isdigit(ch = fgetc(lexi->fptr)))
//                     lexi->buffer[j++] = ch;
//                 if(ch == '.')
//                 {
//                     is_float = 1;
//                     lexi->buffer[j++] = ch;

//                     // Read the fractional part of the number
//                     while (isdigit(ch = fgetc(lexi->fptr)))
//                         lexi->buffer[j++] = ch;
//                 }
//                 lexi->buffer[j++] = '\0';
//                 if(is_float == 0)
//                     printf("Numeric const\t:\t%s\n",lexi->buffer);
//                 else
//                     printf("Float const\t:\t%s\n",lexi->buffer);
//            }
//            j = 0;
//            fseek(lexi->fptr, -1, SEEK_CUR); // Backtrack one character
//         }
//         // if operator
//         else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>' || ch == '%' || 
//                 ch == '&' || ch == '|' || ch == '^' || ch == '!' || ch == '~')
//         {
//             if(ch == '+')
//             {
//                 lexi->buffer[j++] = ch;
//                 char next_ch = fgetc(lexi->fptr);
//                 if(next_ch == '+')
//                     lexi->buffer[j++] = next_ch;
//                 else
//                     fseek(lexi->fptr, -1, SEEK_CUR);
//                 lexi->buffer[j++] = '\0';
//                 printf("Operator\t:\t%s\n",lexi->buffer);
//             }
//             else if(ch == '-')
//             {
//                 lexi->buffer[j++] = ch;
//                 char next_ch = fgetc(lexi->fptr);
//                 if(next_ch == '-')
//                     lexi->buffer[j++] = next_ch;
//                 else
//                     fseek(lexi->fptr, -1, SEEK_CUR);
//                 lexi->buffer[j++] = '\0';
//                 printf("Operator\t:\t%s\n",lexi->buffer);
//             }
//             else
//                 printf("Operator\t:\t%c\n",ch);
//             j = 0;
//         }
//         // If symbols
//         else if(ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']')
//         {
//             printf("Symbols\t\t:\t%c\n",ch);
//         }
//         // if character constant
//         else if(ch == single_quote)
//         {
//             // 'A' this should be print as a character constant
//             ch = fgetc(lexi->fptr);
//             lexi->buffer[0] = ch;
//             lexi->buffer[1] = '\0';
//             char closing_quote = fgetc(lexi->fptr);
//             if(closing_quote != single_quote)
//             {
//                 printf("Character const\t:\tError: Unclosed character literal.\n");
//                 return failure;
//             }
//             else
//                 printf("Character const\t:\t%s\n", lexi->buffer);
//         }
//         // if literals
//         else if(ch == double_quote)
//         {
//             // "Ajinkya" this should be print as a literal
//             int closed = 0;
//             ch = fgetc(lexi->fptr);
//             while(ch != double_quote && ch != '\n' && ch != EOF)
//             {
//                 lexi->buffer[j++] = ch;
//                 ch = fgetc(lexi->fptr);
//             }
//             if(ch == double_quote)
//                 closed = 1;
//             lexi->buffer[j] = '\0';
//             j = 0;
//             if(closed)
//                 printf("String Literal\t:\t%s\n", lexi->buffer);
//             else
//             {
//                 printf("String Literal\t:\tError: Unclosed String literal.\n");
//                 return failure;
//             }
//         }
//         else if(isspace(ch))
//             continue;
//     }
//     fclose(lexi->fptr);
//     return success;
// }
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "lexical.h"

int main(int argc, char *argv[])
{
    Lexical lexi;     // Lexical analyzer object
    Status status;    // Function status

    printf("\n============================================\n");
    printf("       Lexical Analyzer (C Project)     \n");
    printf("============================================\n\n");

    /* Check argument count */
    if (argc != 2)
    {
        printf("Error: Invalid number of arguments passed.\n");
        printf("Usage: ./a.out <file_name.c>\n");
        exit(1);
    }

    /* Validate input file */
    status = read_and_validation_args(argv, &lexi);
    if (status == e_failure)
    {
        printf("\nError: Unable to read or validate input file.\n");
        return 1;
    }

    /* Open source file */
    status = open_file(&lexi);
    if (status == e_failure)
    {
        printf("Error: Unable to open file.\n");
        return 1;
    }

    /* Load C keywords */
    keyword(&lexi);

    /* Start analysis */
    status = start_lexical_analysis(&lexi);
    if (status == e_failure)
    {
    printf("\n============================================\n");

        printf("Error: Lexical analysis failed.\n");
    printf("============================================\n\n");

        return 1;
    }

    fclose(lexi.fptr);   // Close file

    printf("\nLexical analysis completed successfully.\n");
    printf("============================================\n\n");

    return 0;
}

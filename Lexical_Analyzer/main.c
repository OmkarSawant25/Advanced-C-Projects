#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "lexical.h"

int main(int argc, char *argv[])
{
    Lexical lexi;
    Status status;

    printf("\n========================================\n");
    printf("       Lexical Analyzer (C Project)     \n");
    printf("========================================\n\n");

    if (argc != 2)
    {
        printf("Error: Invalid number of argument passed.\n");
        printf("Usage: ./a.out <file_name.c>\n");
        exit(1);
    }

    status = read_and_validation_args(argv, &lexi);
    if (status == e_failure)
    {
        printf("\n❌ Error: Unable to read or validate input file.\n");
        return 1;
    }

    status = open_file(&lexi);
    if (status == e_failure)
    {
        printf("❌ Error: Unable to open file.\n");
        return 1;
    }

    keyword(&lexi);

    status = start_lexical_analysis(&lexi);
    if (status == e_failure)
    {
        printf("❌ Error: Lexical analysis Failed.\n");
        return 1;
    }

    fclose(lexi.fptr);
    printf("\n✅ Lexical analysis completed successfully.\n");

    return 0;
}

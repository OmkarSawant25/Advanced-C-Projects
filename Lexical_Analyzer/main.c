#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "lexical.h"

/* ===== COLOR CODES ===== */
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define WHITE   "\033[1;37m"

int main(int argc, char *argv[])
{
    Lexical lexi;     // Lexical analyzer object
    Status status;    // Function status

    /* Project banner */
    printf("\n%s============================================%s\n", CYAN, RESET);
    printf("%s       Lexical Analyzer (C Project)     %s\n", BLUE, RESET);
    printf("%s============================================%s\n\n", CYAN, RESET);

    /* Check argument count */
    if (argc != 2)
    {
        printf("%s❌ Error:%s Invalid number of arguments passed.\n", RED, RESET);
        printf("%sUsage:%s ./a.out <file_name.c>\n", YELLOW, RESET);
        exit(1);
    }

    /* Validate input file */
    status = read_and_validation_args(argv, &lexi);
    if (status == e_failure)
    {
        printf("\n%s❌ Error:%s Unable to read or validate input file.\n", RED, RESET);
        return 1;
    }

    /* Open source file */
    status = open_file(&lexi);
    if (status == e_failure)
    {
        printf("%s❌ Error:%s Unable to open file.\n", RED, RESET);
        return 1;
    }

    /* Load C keywords */
    keyword(&lexi);

    /* Start analysis */
    status = start_lexical_analysis(&lexi);
    if (status == e_failure)
    {
        printf("%s❌ Error:%s Lexical analysis failed.\n", RED, RESET);
        return 1;
    }

    fclose(lexi.fptr);   // Close file

    /* Success message */
    printf("\n%s✅ Lexical analysis completed successfully.%s\n", GREEN, RESET);
    printf("%s============================================%s\n\n", CYAN, RESET);


    return 0;
}

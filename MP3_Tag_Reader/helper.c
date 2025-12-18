#include "edit.h"
#include "view.h"

Status check_file_extension(char *fname)
{
    // Check for .mp3 extension
    if (strcmp(fname + strlen(fname) - 4, ".mp3") != 0)
    {
        fprintf(stderr, "Error : File should have .mp3 extension\n");
        return FAILURE;
    }
    return SUCCESS;
}

OperationType check_operation_type(char *symbol)
{
    // Check view option
    if (strcmp(symbol, "-v") == 0)
        return VIEW_OPERATION;

    // Check edit option
    else if (strcmp(symbol, "-e") == 0)
        return EDIT_OPERATION;
    
    // Check help option
    else if (strcmp(symbol, "--help") == 0)
        return HELP_OPERATION;

    // Unsupported option
    else
        return UNSUPPORTED;
}

void display_help()
{
    // Help banner
    printf("\n                 -> MP3 TAG READER - HELP <-             \n\n");

    // Usage section
    printf("-----------------------------------------------------------------\n");
    printf("-> USAGE:\n");
    printf("-----------------------------------------------------------------\n");
    printf("1. To view metadata:\n");
    printf("      ./a.out -v <mp3_filename>\n");
    printf("2. To edit metadata:\n");
    printf("      ./a.out -e -t/-a/-A/-m/-y/-c <new_file> <mp3_filename>\n");
    printf("3. For help:\n");
    printf("      ./a.out --help\n");

    // Options section
    printf("\n-----------------------------------------------------------------\n");
    printf("-> Options:\n");
    printf("-----------------------------------------------------------------\n");
    printf("  -v           View MP3 tag information\n");
    printf("  -e           Edit MP3 tag information\n");
    printf("  --help       Display this help menu\n\n");

    // Edit options
    printf("-----------------------------------------------------------------\n");
    printf("-> Edit Options (used with -e):\n");
    printf("-----------------------------------------------------------------\n");
    printf("  -t           Edit Title tag\n");
    printf("  -a           Edit Artist tag\n");
    printf("  -A           Edit Album tag\n");
    printf("  -y           Edit Year tag\n");
    printf("  -m           Edit Content tag\n");
    printf("  -c           Edit Comment/Composer tag\n");
    printf("-----------------------------------------------------------------\n\n");
}

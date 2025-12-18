#include "types.h"
#include "view.h"
#include "edit.h"

int main(int argc, char *argv[])
{
    // Project banner
    printf("\n=================================================================\n");
    printf("               MP3 TAG READER & EDITOR FOR ID3v2\n");
    printf("=================================================================\n");

    // Check minimum arguments
    if (argc < 2)
    {
        // Usage instructions
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE:\n");
        printf("  To view metadata:\n");
        printf("      ./a.out -v <mp3_filename>\n\n");
        printf("  To edit metadata:\n");
        printf("      ./a.out -e -t/-a/-A/-m/-y/-c <new_file> <mp3_filename>\n\n");
        printf("  For help:\n");
        printf("      ./a.out --help\n");

        printf("=================================================================\n");
        return FAILURE;
    }

    // Identify requested operation
    OperationType op_type = check_operation_type(argv[1]);

    if (op_type == VIEW_OPERATION)        // View MP3 metadata
    {
        ViewInfo view;

        if (validate_view_arguments(&view, argc, argv) == SUCCESS) // Validate view args
        {
            if (start_view(&view) == FAILURE) // Read and display tags
            {
                fprintf(stderr, "Error : Failed to View metadata.\n");
            }
        }
        else
        {
            fprintf(stderr, "Error : View Validations Failed\n");
            printf("=================================================================\n\n");
        }
    }
    else if (op_type == EDIT_OPERATION)   // Edit MP3 metadata
    {
        EditInfo edit;

        if (validate_edit_arguments(&edit, argc, argv) == SUCCESS) // Validate edit args
        {
            if (start_edit(&edit) == FAILURE) // Modify tag data
            {
                fprintf(stderr, "Error : Failed to Edit metadata.\n");
            }
        }
        else
        {
            fprintf(stderr, "Error : Edit Validations Failed\n");
        }
    }
    else if (op_type == HELP_OPERATION)   // Show help menu
    {
        display_help();
    }
}

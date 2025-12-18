#include "view.h"

Status validate_view_arguments(ViewInfo *view, int argc, char *argv[])
{
    char ID[4];   // To store ID3 header

    if (argc == 3)    // Check correct number of arguments
    {
        view->file_name = argv[2];   // Store MP3 file name

        if (check_file_extension(view->file_name) == FAILURE)  // Check .mp3 extension
            return FAILURE;

        view->fptr_mp3 = fopen(view->file_name, "rb"); // Open MP3 file

        if (view->fptr_mp3 == NULL)   // File open failure
        {
            fprintf(stderr, "Error : Failed to open file %s\n", view->file_name);
            return FAILURE;
        }

        fread(ID, 3, 1, view->fptr_mp3); // Read ID3 tag
        ID[3] = '\0';

        if (strcmp(ID, "ID3") != 0)   // Check valid MP3 file
        {
            fprintf(stderr, "Error : MP3 File is not present\n");
            return FAILURE;
        }
    }
    else
    {
        fprintf(stderr, "Error : Invalid Number of arguments\n"); // Wrong input format
        return FAILURE;
    }
    return SUCCESS;
}

Status start_view(ViewInfo *view)
{
    fseek(view->fptr_mp3, 7, SEEK_CUR); // Skip MP3 header info

    for (int i = 0; i < 6; i++)        // Read 6 ID3 frames
    {
        fread(view->tag, 4, 1, view->fptr_mp3); // Read tag name
        view->tag[4] = '\0';

        fread(&view->size, 4, 1, view->fptr_mp3); // Read tag size

        Big_Endian_to_Little_Endian(&view->size); // Convert size format

        fseek(view->fptr_mp3, 3, SEEK_CUR); // Skip frame flags

        char content[view->size + 1]; // Buffer for tag data

        fread(content, view->size - 1, 1, view->fptr_mp3); // Read tag content
        content[view->size - 1] = '\0';

        print_function(view->tag, content, view->size); // Print tag info
    }

    printf("=================================================================\n\n");
}

void Big_Endian_to_Little_Endian(unsigned int *size)
{
    // Convert big endian to little endian
    *size = (*size & 0XFF) << 24 |
            (*size & 0XFF00) << 8 |
            (*size & 0XFF0000) >> 8 |
            (*size & 0XFF000000) >> 24;
}

void display(char *data, int size)
{
    for (int j = 0; j < size - 1; j++) // Print content characters
    {
        printf("%c", data[j]);
    }
    printf("\n");
}

void print_function(char *tag, char *content, int size)
{
    if (strcmp(tag, "TPE1") == 0)          // Artist name
    {
        printf("%-15s : ", "Artist");
        display(content, size);
    }
    else if (strcmp(tag, "TIT2") == 0)     // Song title
    {
        printf("%-15s : ", "Title");
        display(content, size);
    }
    else if (strcmp(tag, "TALB") == 0)     // Album name
    {
        printf("%-15s : ", "Album");
        display(content, size);
    }
    else if (strcmp(tag, "TYER") == 0)     // Year
    {
        printf("%-15s : ", "Year");
        display(content, size);
    }
    else if (strcmp(tag, "TCON") == 0)     // Content type
    {
        printf("%-15s : ", "Content Type");
        display(content, size);
    }
    else if (strcmp(tag, "COMM") == 0)     // Comment
    {
        printf("%-15s : ", "Comment");
        display(content, size);
    }
    else if (strcmp(tag, "TCOM") == 0)     // Composer
    {
        printf("%-15s : ", "Composer");
        display(content, size);
    }
}

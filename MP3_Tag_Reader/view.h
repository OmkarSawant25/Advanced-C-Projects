#ifndef VIEW_H
#define VIEW_H

#include "types.h"

#define TAG_SIZE 5   // Size to store ID3 tag name

typedef struct view
{
    FILE *fptr_mp3;          // MP3 file pointer
    char *file_name;         // MP3 file name
    char tag[TAG_SIZE];      // ID3 tag (TIT2, TPE1, etc.)
    unsigned int size;       // Size of tag data

} ViewInfo;

// Validate arguments and MP3 file
Status validate_view_arguments(ViewInfo *view, int argc, char *argv[]);

// Read and display MP3 tag information
Status start_view(ViewInfo *view);

// Convert size from big endian to little endian
void Big_Endian_to_Little_Endian(unsigned int *size);

// Match tag and print corresponding data
void print_function(char *tag, char *content, int size);

// Display tag content
void display(char *data, int size);

#endif

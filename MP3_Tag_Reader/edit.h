#ifndef EDIT_H
#define EDIT_H

#include "types.h"

#define TAG_SIZE 5   // Size for ID3 tag name

typedef struct edit
{
    FILE *fptr_src;            // Source MP3 file
    char *fname_src;           // Source file name
    FILE *fptr_dest;           // Destination MP3 file
    char *fname_dest;          // Output file name
    char tag[TAG_SIZE];        // ID3 tag to edit
    char edit_data[100];       // New tag data
    unsigned int edit_data_size; // Size of new data

} EditInfo;

// Check whether file has .mp3 extension
Status check_file_extension(char *fname);

// Get size of MP3 file
long validate_file_size(FILE *fptr);

// Validate edit command arguments
Status validate_edit_arguments(EditInfo *edit, int argc, char *argv[]);

// Check which tag needs to be edited
Status check_tag(EditInfo *edit, char *argv);

// Start editing process
Status start_edit(EditInfo *edit);

// Copy MP3 header data
Status copy_header_file(EditInfo *edit);

// Copy tag size and data
void copy_tag_size_data(char read_tag[], EditInfo *edit);

// Copy remaining MP3 data
void copy_remaining_data(EditInfo *edit);

#endif

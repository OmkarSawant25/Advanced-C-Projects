#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Status of function execution
typedef enum
{
    FAILURE,    // Operation failed
    SUCCESS     // Operation successful
} Status;

// Type of operation selected by user
typedef enum
{
    VIEW_OPERATION,     // View MP3 tags
    EDIT_OPERATION,     // Edit MP3 tags
    HELP_OPERATION,     // Display help menu
    UNSUPPORTED         // Invalid option
} OperationType;

// Identify operation from command-line argument
OperationType check_operation_type(char *symbol);

// Check whether file extension is valid
Status check_file_extension(char *fname);

// Display help menu
void display_help();

#endif
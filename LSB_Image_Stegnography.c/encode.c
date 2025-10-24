#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    // Find the size of secret file data
    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    return size;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Check whether the file name is present or not (before .)
    if (argv[2][0] != '.')
    {
        // Validate Source File extension (.bmp) or not.
        if (strstr(argv[2], ".bmp") != NULL)
        {
            // encInfo -> src_image_fname = argv[2]
            encInfo->src_image_fname = argv[2];
        }
        else
        {
            fprintf(stderr, "Error: '%s' is not a valid source file. Please provide a file with a .bmp extension.\n", argv[2]);
            return e_failure;
        }
    }
    else
    {
        fprintf(stderr, "Error: File name is missing before the dot ('.').\n");

        return e_failure;
    }

    // Check whether the file name is present or not (before .)
    if (argv[3][0] != '.')
    {
        // Validate Secret File extension (.txt or .c or .h or .sh) or not.
        if ((strstr(argv[3], ".txt") != NULL) || (strstr(argv[3], ".c") != NULL) || (strstr(argv[3], ".h") != NULL) || (strstr(argv[3], ".sh") != NULL))
        {
            // encInfo -> secret_fname = argv[3]
            encInfo->secret_fname = argv[3];
        }
        else
        {
            fprintf(stderr, "Error: '%s' is not a valid secret file. Please provide a file with a .txt or .c or .sh or .h extension.\n", argv[3]);
            return e_failure;
        }
    }
    else
    {
        fprintf(stderr, "Error: File name is missing before the dot ('.').\n");

        return e_failure;
    }

    // Check whether the argv[4] is having NULL or not
    if (argv[4] == NULL)
    {
        // encInfo -> stego_image_fname = "destination.bmp"
        encInfo->stego_image_fname = "destination.bmp";
    }
    else
    {
        // Check whether the file name is present or not (before .)
        if (argv[4][0] != '.')
        {
            // Validate Destination File extension (.bmp) or not.
            if (strstr(argv[4], ".bmp") != NULL)
            {
                // encInfo -> stego_image_fname = argv[4]
                encInfo->stego_image_fname = argv[4];
            }
            else
            {
                fprintf(stderr, "Error: '%s' is not a valid destination file. Please provide a file with a .bmp extension.\n", argv[4]);
                return e_failure;
            }
        }
        else
        {
            fprintf(stderr, "Error: File name is missing before the dot ('.').\n");
            return e_failure;
        }
    }
    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    char *extn;
    if (strstr(encInfo->secret_fname, ".txt") != NULL)
        extn = strstr(encInfo->secret_fname, ".txt");
    else if (strstr(encInfo->secret_fname, ".c") != NULL)
        extn = strstr(encInfo->secret_fname, ".c");
    else if (strstr(encInfo->secret_fname, ".h") != NULL)
        extn = strstr(encInfo->secret_fname, ".h");
    else if (strstr(encInfo->secret_fname, ".sh") != NULL)
        extn = strstr(encInfo->secret_fname, ".sh");

    int extn_size = strlen(extn);
    int total_bytes = 54 + (strlen(MAGIC_STRING) * 8) + 32 + (extn_size * 8) + 32 + (encInfo->size_secret_file * 8);

    if (encInfo->image_capacity > total_bytes)
        return e_success;
    else
        return e_failure;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    fread(buffer, 54, 1, fptr_src_image);
    fwrite(buffer, 54, 1, fptr_dest_image);
    if (ftell(fptr_src_image) == ftell(fptr_dest_image))
        return e_success;
    else
        return e_failure;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
}

Status encode_size_to_lsb(int size, char *imageBuffer)
{
}

Status do_encoding(EncodeInfo *encInfo)
{
    // Call the open_files(encInfo)
    // Use nested if
    //
}

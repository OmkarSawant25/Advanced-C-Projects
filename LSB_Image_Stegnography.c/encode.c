#include <stdio.h>
#include <string.h>
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

int extn_size;

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

Status validate_file_extension(const char *filename, char *valid_extns[], int extn_count)
{
    if (filename[0] == '.')
    {
        fprintf(stderr, "Error: Invalid file name '%s' — missing name before '.'\n", filename);
        return e_failure;
    }

    char *dot = strrchr(filename, '.');
    if (dot == NULL)
    {
        fprintf(stderr, "Error: File '%s' has no extension.\n", filename);
        return e_failure;
    }

    for (int i = 0; i < extn_count; i++)
    {
        if (strcmp(dot, valid_extns[i]) == 0)
            return e_success;
    }

    fprintf(stderr, "Error: '%s' has invalid extension.\n", filename);
    return e_failure;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char *bmp_ext[] = {".bmp"};
    if (validate_file_extension(argv[2], bmp_ext, 1) == e_success)
    {
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        fprintf(stderr, "Error: Invalid source file '%s'. Must be a .bmp file.\n", argv[2]);
        return e_failure;
    }

    char *secret_ext[] = {".txt", ".c", ".h", ".sh"};
    if (validate_file_extension(argv[3], secret_ext, 4) == e_success)
    {
        encInfo->secret_fname = argv[3];
    }
    else
    {
        fprintf(stderr, "Error: Invalid secret file '%s'. Must be .txt, .c, .h, or .sh.\n", argv[3]);
        return e_failure;
    }

    if (argv[4] == NULL)
    {
        encInfo->stego_image_fname = "destination.bmp";
    }
    else
    {
        if (validate_file_extension(argv[4], bmp_ext, 1) == e_success)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            fprintf(stderr, "Error: Invalid output file '%s'. Must be a .bmp file.\n", argv[4]);
            return e_failure;
        }
    }
    return e_success;
}

// Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
// {
//     // Check whether the file name is present or not (before .)
//     if (argv[2][0] != '.')
//     {
//         // Validate Source File extension (.bmp) or not.
//         if (strstr(argv[2], ".bmp") != NULL)
//         {
//             // encInfo -> src_image_fname = argv[2]
//             encInfo->src_image_fname = argv[2];
//         }
//         else
//         {
//             fprintf(stderr, "Error: '%s' is not a valid source file. Please provide a file with a .bmp extension.\n", argv[2]);
//             return e_failure;
//         }
//     }
//     else
//     {
//         fprintf(stderr, "Error: File name is missing before the dot ('.').\n");
//         return e_failure;
//     }

//     // Check whether the file name is present or not (before .)
//     if (argv[3][0] != '.')
//     {
//         // Validate Secret File extension (.txt or .c or .h or .sh) or not.
//         if ((strstr(argv[3], ".txt") != NULL) || (strstr(argv[3], ".c") != NULL) || (strstr(argv[3], ".h") != NULL) || (strstr(argv[3], ".sh") != NULL))
//         {
//             // encInfo -> secret_fname = argv[3]
//             encInfo->secret_fname = argv[3];
//         }
//         else
//         {
//             fprintf(stderr, "Error: '%s' is not a valid secret file. Please provide a file with a .txt or .c or .sh or .h extension.\n", argv[3]);
//             return e_failure;
//         }
//     }
//     else
//     {
//         fprintf(stderr, "Error: File name is missing before the dot ('.').\n");

//         return e_failure;
//     }

//     // Check whether the argv[4] is having NULL or not
//     if (argv[4] == NULL)
//     {
//         // encInfo -> stego_image_fname = "destination.bmp"
//         encInfo->stego_image_fname = "destination.bmp";
//     }
//     else
//     {
//         // Check whether the file name is present or not (before .)
//         if (argv[4][0] != '.')
//         {
//             // Validate Destination File extension (.bmp) or not.
//             if (strstr(argv[4], ".bmp") != NULL)
//             {
//                 // encInfo -> stego_image_fname = argv[4]
//                 encInfo->stego_image_fname = argv[4];
//             }
//             else
//             {
//                 fprintf(stderr, "Error: '%s' is not a valid destination file. Please provide a file with a .bmp extension.\n", argv[4]);
//                 return e_failure;
//             }
//         }
//         else
//         {
//             fprintf(stderr, "Error: File name is missing before the dot ('.').\n");
//             return e_failure;
//         }
//     }
//     return e_success;
// }

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
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

    strcpy(encInfo->extn_secret_file, extn);
    // printf("%s\n",encInfo->extn_secret_file);
    extn_size = strlen(extn);
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
    char buffer[8];
    for (size_t i = 0; i < strlen(magic_string); i++)
    {
        fread(buffer, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i], buffer);
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(size, buffer);
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];
    for (size_t i = 0; i < strlen(file_extn); i++)
    {
        fread(buffer, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i], buffer);
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, buffer);
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    fread(encInfo->secret_data, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    char buffer[8];
    for (size_t i = 0; i < encInfo->size_secret_file; i++)
    {
        fread(buffer, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(encInfo->secret_data[i], buffer);
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src) == 1)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    // for char
    for (size_t i = 0; i < 8; i++)
    {
        image_buffer[i] = image_buffer[i] & (~1) | (data >> i) & 1;
    }
    return e_success;
}

Status encode_size_to_lsb(int size, char *imageBuffer)
{
    // for integer
    for (size_t i = 0; i < 32; i++)
    {
        imageBuffer[i] = (imageBuffer[i] & (~1)) | ((size >> i) & 1);
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    printf("Starting encoding process.\n");

    // Step 1: Open files
    if (open_files(encInfo) == e_success)
    {
        printf("Opened required files successfully.\n");

        // Step 2: Check capacity
        if (check_capacity(encInfo) == e_success)
        {
            printf("Source image has sufficient capacity.\n");

            // Step 3: Copy BMP header
            if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
            {
                printf("BMP header copied successfully.\n");

                // Step 4: Encode magic string
                if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("Magic string encoded successfully.\n");

                    // Step 5: Encode secret file extension size
                    if (encode_secret_file_extn_size(extn_size, encInfo) == e_success)
                    {
                        printf("Secret file extension size encoded successfully.\n");

                        // Step 6: Encode secret file extension
                        if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                        {
                            printf("Secret file extension encoded successfully.\n");

                            // Step 7: Encode secret file size
                            if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                            {
                                printf("Secret file size encoded successfully.\n");

                                // Step 8: Encode secret file data
                                if (encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("Secret file data encoded successfully.\n");

                                    // Step 9: Copy remaining image data
                                    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                    {
                                        printf("Remaining image data copied successfully.\n");
                                        return e_success;
                                    }
                                    else
                                    {
                                        printf("ERROR: Copying remaining image data failed!\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("ERROR: Encoding secret file data failed!\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("ERROR: Encoding secret file size failed!\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("ERROR: Encoding secret file extension failed!\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("ERROR: Encoding secret file extension size failed!\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("ERROR: Encoding magic string failed!\n");
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR: Copying BMP header failed!\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR: Source image does not have enough capacity to encode data.\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Opening files failed!\n");
        return e_failure;
    }
    return e_failure;
}

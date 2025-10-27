#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

Status validate_file_extension_decode(const char *filename, char *valid_extns[], int extn_count)
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

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char *bmp_ext[] = {".bmp"};

    if (validate_file_extension_decode(argv[2], bmp_ext, 1) == e_success)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("Error: '%s' has invalid extension.\n", argv[2]);
    }

    if (argv[3] == NULL)
    {
        decInfo->secret_fname = "Decoded";
    }
    else
    {
        char *str;
        str = strtok(argv[3], ".");
        decInfo->secret_fname = str;
    }
    return e_success;
}

Status open_decoded_files(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

        return e_failure;
    }
    // No failure return e_success
    return e_success;
}

Status skip_bmp_header(FILE *fptr_dest_image)
{
    fseek(fptr_dest_image, 54, SEEK_SET);
    return e_success;
}

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char string[20];
    char ch;
    char buffer[8];
    int i;
    for (i = 0; i < 2; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&ch, buffer);
        string[i] = ch;
    }
    string[i] = '\0';
    // printf("Magic %s\n", string);
    if (strcmp(MAGIC_STRING, string) == 0)
    {
        return e_success;
    }
    return e_failure;
}
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    char new = 0;
    for (size_t i = 0; i < 8; i++)
    {
        new = new | (image_buffer[i] & 1) << i;
    }
    *data = new;
    return e_success;
}

Status decode_size_from_lsb(int *size, char *imageBuffer)
{
    int new_size = 0;
    for (size_t i = 0; i < 32; i++)
    {
        new_size = new_size | (imageBuffer[i] & 1) << i;
    }
    *size = new_size;
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int size;
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    decode_size_from_lsb(&size, buffer);
    decInfo->ext_size = (long)size;
    printf("Extern size %ld\n", decInfo->ext_size);
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];
    char extn[5];
    char ch;
    int i;
    for (i = 0; i < decInfo->ext_size; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&ch, buffer);
        extn[i] = ch;
    }
    extn[i] = '\0';
    char *p;
    p = strcat(decInfo->secret_fname,extn);
    // printf("Extn %s\n", decInfo->secret_fname);
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int size;
    fread(buffer,32,1,decInfo->fptr_stego_image);
    decode_size_from_lsb(&size,buffer);
    printf("%d\n",size);
    decInfo->size_secret_file = size;
    return e_success;

}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char ch;
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    for(int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(buffer,8,1,decInfo->fptr_stego_image);
        decode_byte_from_lsb(&ch, buffer);
        // fprintf(decInfo->fptr_secret,"%c",ch);
        fwrite(&ch,1,1,decInfo->fptr_secret);
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (decode_secret_file_extn_size(decInfo) == e_success)
    {
        if (decode_secret_file_extn(decInfo) == e_success)
        {
            if(decode_secret_file_size(decInfo) == e_success)
            {
                if(decode_secret_file_data(decInfo) == e_success)
                    return e_success;
            }
        }
    }
}
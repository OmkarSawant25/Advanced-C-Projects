#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include "common.h"

OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
    printf("\n========================================\n");
    printf(" 🔐  Steganography using LSB Technique\n");
    printf("========================================\n\n");

    // Step 1: Check for minimum argument count
    if (argc >= 3)
    {
        // Step 2: Check whether encode or decode
        OperationType op_type = check_operation_type(argv[1]);

        /*------- ENCODING SECTION -------*/
       
        if (op_type == e_encode)
        {
            printf("🔒 Selected Encoding Operation\n\n");

            // Step 3: Declare structure variable EncodeInfo
            EncodeInfo enc_info;

            // Step 4: Validate and read encode arguments
            if (read_and_validate_encode_args(argv, &enc_info) == e_success)
            {
                // printf("Encode arguments validated successfully.\n");
                printf("-> Encode arguments validated successfully.\n");

                // Step 5: Call do_encoding
                if (do_encoding(&enc_info) == e_success)
                {
                    printf("\n✅ Encoding completed successfully!\n");
                    printf("📁 Output file generated: %s\n", enc_info.stego_image_fname);
                }
                else
                {
                    printf("\n❌ ERROR: Encoding failed.\n");
                }
            }
            else
            {
                printf("❌ ERROR: Invalid encode arguments.\n");
            }
        }
        /*------- DECODING SECTION -------*/

        else if (op_type == e_decode)
        {
            printf("🔓 Selected decoding operation.\n\n");

            // Step 3: Declare structure variable DecodeInfo
            DecodeInfo dec_info;

            // Step 4: Validate and read decode arguments
            if (read_and_validate_decode_args(argv, &dec_info) == e_success)
            {
                printf("-> Decode arguments validated successfully.\n");

                // Step 5: Open stego image file
                if (open_decoded_files(&dec_info) == e_success)
                {
                    // Step 6: Skip BMP header (first 54 bytes)
                    skip_bmp_header(dec_info.fptr_stego_image);

                    // Step 7: Verify magic string
                    if (decode_magic_string(MAGIC_STRING, &dec_info) == e_success)
                    {
                        // Step 8: Perform decoding process
                        if (do_decoding(&dec_info) == e_success)
                        {
                            printf("\n✅ Decoding completed successfully!\n");
                            printf("📁 Output file generated: %s\n", dec_info.secret_fname);

                        }
                        else
                        {
                            printf("\n❌ ERROR: Decoding failed.\n");
                        }
                    }
                    else
                    {
                        printf("❌ ERROR: Provided image is not an encoded file.\n");
                    }
                }
            }
            else
            {
                printf("❌ ERROR: Invalid decode arguments.\n");
            }
        }
        else
        {
            printf("❌ ERROR: Unsupported operation type.\n");
            printf("Use -e for encode or -d for decode.\n");
        }
    }

    // Step 9: Handle invalid or insufficient arguments
    else
    {
        printf("❌ ERROR: Invalid number of arguments.\n\n");
        printf("Usage:\n");
        printf(" 🔎 To Encode: %s -e <source_image.bmp> <secret_file.txt> [output_image.bmp]\n", argv[0]);
        printf(" 🔎 To Decode: %s -d <stego_image.bmp> [output_file_name]\n", argv[0]);
    }
    printf("========================================\n\n");


    return 0;
}

//  * Function: check_operation_type
//  * Description: Determines whether user selected encode or decode.

OperationType check_operation_type(char *symbol)
{
    // Step 1: Check whether the symbol is -e or not
    if (strcmp(symbol, "-e") == 0)
        return e_encode;

    // Step 2: Check whether the symbol is -d or not
    else if (strcmp(symbol, "-d") == 0)
        return e_decode;

    // Step 3: Otherwise, return unsupported
    else
        return e_unsupported;
}

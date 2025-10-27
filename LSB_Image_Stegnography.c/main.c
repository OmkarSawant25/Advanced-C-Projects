#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
    // Step 1: Check for minimum argument count
    if (argc >= 4)
    {
        // Step 2: Check whether encode or decode
        OperationType op_type = check_operation_type(argv[1]);

        if (op_type == e_encode)
        {
            // Encoding section
            printf("Selected encoding operation.\n");

            // Step 3: Declare structure variable EncodeInfo
            EncodeInfo enc_info;

            // Step 4: Validate and read encode arguments
            if (read_and_validate_encode_args(argv, &enc_info) == e_success)
            {
                printf("Encode arguments validated successfully.\n");

                // Step 5: Call do_encoding
                if (do_encoding(&enc_info) == e_success)
                {
                    printf("Encoding completed successfully!\n");
                }
                else
                {
                    printf("ERROR: Encoding failed.\n");
                }
            }
            else
            {
                printf("ERROR: Invalid encode arguments.\n");
            }
        }
        else
        {
            printf("ERROR: Unsupported operation type. Use -e for encode or -d for decode.\n");
        }
    }
    else
    {
        printf("ERROR: Invalid number of arguments.\n");
    }
}

OperationType check_operation_type(char *symbol)
{
    // Step 1 : Check whether the symbol is -e or not true - > return e_encode false -> Step 2
    if (strcmp(symbol, "-e") == 0)
        return e_encode;
    // Step 2 : Check whether the symbol is -d or not true - > return e_decode false -> return e_unsupported
    else if (strcmp(symbol, "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}

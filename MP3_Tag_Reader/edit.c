#include "edit.h"
#include "view.h"

Status validate_edit_arguments(EditInfo *edit, int argc, char *argv[])
{
    char ID[4];   // To store ID3 header

    if (argc == 5)    // Check correct argument count
    {
        if (check_tag(edit, argv[2]) == FAILURE) // Validate edit tag
        {
            return FAILURE;
        }

        edit->fname_src = argv[4];          // Source MP3 file
        edit->fname_dest = "destination.mp3"; // Output file name

        if (check_file_extension(edit->fname_src) == FAILURE) // Check .mp3
        {
            return FAILURE;
        }

        edit->fptr_src = fopen(edit->fname_src, "rb"); // Open source file
        edit->fptr_dest = fopen(edit->fname_dest, "wb"); // Open destination file

        if (edit->fptr_src == NULL) // Source open failed
        {
            fprintf(stderr, "Error : Failed to open file %s\n", edit->fname_src);
            return FAILURE;
        }

        if (edit->fptr_dest == NULL) // Destination open failed
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Failed to open file %s\n", edit->fname_dest);
            return FAILURE;
        }

        if (validate_file_size(edit->fptr_src) == 0) // Check empty file
        {
            fprintf(stderr, "Error : File '%s' is Empty\n", edit->fname_src);
            return FAILURE;
        }

        fread(ID, 3, 1, edit->fptr_src); // Read ID3 header
        ID[3] = '\0';

        if (strcmp(ID, "ID3") != 0) // Validate MP3 file
        {
            fprintf(stderr, "Error : MP3 File is not present\n");
            return FAILURE;
        }

        strcpy(edit->edit_data, argv[3]);          // Store new tag data
        edit->edit_data_size = strlen(edit->edit_data) + 1; // Data size
    }
    else
    {
        fprintf(stderr, "Error : Invalid Number of arguments\n"); // Wrong input
        return FAILURE;
    }
    return SUCCESS;
}

Status check_tag(EditInfo *edit, char *argv)
{
    if (!strcmp(argv, "-t"))        // Title
        strcpy(edit->tag, "TIT2");
    else if (!strcmp(argv, "-a"))   // Artist
        strcpy(edit->tag, "TPE1");
    else if (!strcmp(argv, "-A"))   // Album
        strcpy(edit->tag, "TALB");
    else if (!strcmp(argv, "-y"))   // Year
        strcpy(edit->tag, "TYER");
    else if (!strcmp(argv, "-m"))   // Content
        strcpy(edit->tag, "TCON");
    else if (!strcmp(argv, "-c"))   // Comment
        strcpy(edit->tag, "COMM");
    else
    {
        printf("Error : Invalid tag. Please use ./a.out --help for correct format\n");
        return FAILURE;
    }
    return SUCCESS;
}

long validate_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END); // Move to end
    long size = ftell(fptr);  // Get file size
    rewind(fptr);             // Reset file pointer
    return size;
}

Status start_edit(EditInfo *edit)
{
    copy_header_file(edit); // Copy ID3 header

    char read_tag_from_src[5]; // Buffer for tag

    for (int i = 0; i < 6; i++) // Process 6 frames
    {
        fread(read_tag_from_src, 4, 1, edit->fptr_src); // Read tag
        read_tag_from_src[4] = '\0';

        if (strcmp(edit->tag, read_tag_from_src) == 0) // Target tag found
        {
            unsigned int src_size;
            unsigned int dest_size;
            char flag_buffer[4];

            fwrite(read_tag_from_src, 4, 1, edit->fptr_dest); // Write tag

            fread(&src_size, 4, 1, edit->fptr_src); // Read old size
            Big_Endian_to_Little_Endian(&src_size);

            dest_size = edit->edit_data_size; // New size
            Big_Endian_to_Little_Endian(&dest_size);
            fwrite(&dest_size, 4, 1, edit->fptr_dest); // Write new size

            fread(flag_buffer, 3, 1, edit->fptr_src); // Read flags
            fwrite(flag_buffer, 3, 1, edit->fptr_dest); // Write flags

            fwrite(edit->edit_data, edit->edit_data_size - 1, 1, edit->fptr_dest); // Write new data

            fseek(edit->fptr_src, src_size - 1, SEEK_CUR); // Skip old data
            break;
        }
        else
        {
            copy_tag_size_data(read_tag_from_src, edit); // Copy unchanged tag
        }
    }

    copy_remaining_data(edit); // Copy rest of file

    remove(edit->fname_src);
    rename(edit->fname_dest, edit->fname_src);
    return SUCCESS;
}

Status copy_header_file(EditInfo *edit)
{
    fseek(edit->fptr_src, 0, SEEK_SET); // Go to file start
    char buffer[10];
    fread(buffer, 10, 1, edit->fptr_src); // Read header
    fwrite(buffer, 10, 1, edit->fptr_dest); // Write header
    return SUCCESS;
}

void copy_tag_size_data(char read_tag[], EditInfo *edit)
{
    unsigned int size;
    char flags[2];

    fwrite(read_tag, 4, 1, edit->fptr_dest); // Write tag

    fread(&size, 4, 1, edit->fptr_src); // Read size
    fwrite(&size, 4, 1, edit->fptr_dest); // Write size

    Big_Endian_to_Little_Endian(&size); // Convert size

    fread(flags, 2, 1, edit->fptr_src); // Read flags
    fwrite(flags, 2, 1, edit->fptr_dest); // Write flags

    char buffer[size]; // Buffer for data
    fread(buffer, size, 1, edit->fptr_src); // Read data
    fwrite(buffer, size, 1, edit->fptr_dest); // Write data
}

void copy_remaining_data(EditInfo *edit)
{
    char ch;
    while (fread(&ch, 1, 1, edit->fptr_src) > 0) // Read byte by byte
        fwrite(&ch, 1, 1, edit->fptr_dest);      // Write byte
}
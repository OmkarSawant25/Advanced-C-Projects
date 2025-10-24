#include <stdio.h>
#include <string.h>

// defining main with arguments
int main(int argc, char *argv[])
{
    printf("You have entered %d arguments:\n", argc);

    // if (strcmp(argv[1], "-el") == 0)
    //     printf("TRUE\n");

    // for (int i = 0; i < argc; i++)
    // {
    //     printf("%c\n", argv[i][0]);
    // }
    if(argv[1] == NULL)
    {
        printf("Null\n");
    }
    else
    {
        printf("%s\n", argv[1]);
    }

    return 0;
}
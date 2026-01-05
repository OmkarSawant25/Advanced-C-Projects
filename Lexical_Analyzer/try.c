#include <stdio.h>

int main()
{
    char a = 'x';      // valid
    char b = '\n';     // valid escape

    // char c = 'y;       // error: missing closing quote
    // char d = '';       // error: empty character constant
    // char e = 'ab';     // error: more than one character
    char c = 'y';       // error: missing closing quote


    return 0;
}
#include <stdio.h>
#include "math.h"

#define PI 3.14159

// This is a single-line comment

/*
   This is a multi-line comment
   It should be skipped by the lexical analyzer
*/

int main()
{
    int a = 10;      // Decimal constant
    float b = 12.34; // Floating constant
    int c = 075;     // Octal constant
    int d = 0x1A3F;  // Hexadecimal constant
    char e = 'A';    // Character constant
    char f = '\n';   // Escape character constant
    int g = 0b101010101;
    char str[] = "Hello, World!"; // String literal

    // Operators
    a++;       // Increment
    b -= 2.5;  // Compound operator
    c = a + b; // Arithmetic
    d *= c;    // Multiplication assignment
    if (a >= b && c != d)
    {
        printf("a = %d, b = %f\n", a, b);
    }
    else
    {
        printf("Condition failed\n");
    }

    // Keywords and Identifiers
    for (int i = 0; i < 5; i++)
    {
        printf("Loop iteration: %d\n", i);
    }

    while (a > 0)
    {
        a--;
    }

    return 0;
}

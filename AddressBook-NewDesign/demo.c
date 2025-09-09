#include<stdio.h>
#include<string.h>
int main()
{
    char s1[20] = "Omkar";
    char s2[20] = "Omkar";

    int a = strcasecmp(s1, s2);
    printf("%d\n", a);

	return 0;
}

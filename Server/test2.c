#include <stdio.h>
#include <string.h>

int main()
{
    char a[4];
    char b[5];

    a[0] = 'a';
    a[1] = 'b';
    a[2] = 'c';
    a[3] = '\0';

    b[0] = 'a';
    b[1] = 'b';
    b[2] = 'c';
    b[3] = 'd';
    b[4] = '\0';

    if (strcmp(a, b) == 0)
    {
        printf("same\n");
    }
    else
    {
        printf("different\n");
    }

    return 0;
}

// ���H�w��b���� 2020/05/11.
// eval.c
//
#include <stdio.h>
#include <stdlib.h>

void eval(int a, int b)
{
    if (a > b)
    {
        fprintf(stderr, "%d > %d\n", a, b);
    }
    else if (a < b)
    {
        fprintf(stderr, "%d < %d\n", a, b);
    }
    else
    {
        fprintf(stderr, "%d == %d\n", a, b);
    }

    return;
}

// î•ñHŠwŠî‘bÀŒ± 2020/05/11.
// main.c
//
#include <stdio.h>
#include <stdio.h>

void eval(int x, int y);

int main(int argc, char *argv[])
{
    int a = 0, b = 0;
    fprintf(stderr, "Input 2 Numbers: ");
    fscanf(stdin, "%d %d", &a, &b);

    eval(a, b);

    return 0;
}
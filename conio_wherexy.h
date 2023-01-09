
#define CONIO_H


#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


int wherexy(int *x, int *y)
{
    printf("\033[6n");
    if(getch() != '\x1B') return 0;
    if(getch() != '\x5B') return 0;
    int in;
    int ly = 0;
    while((in = getch()) != ';')
        ly = ly * 10 + in - '0';
    int lx = 0;
    while((in = getch()) != 'R')
        lx = lx * 10 + in - '0';
    *x = lx;
    *y = ly;
}
int wherex()
{
    int x=0,y=0;
    wherexy(&x, &y);
    return x;
}

int wherey()
{
    int x=0,y=0;
    wherexy(&x, &y);
    return y;
}
#include "messageSys.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int encode(int x, int y) 
{
int comb = x * (5 + 1) + y;
return comb;
}


int decodeX(int comb)
{
int x_extr = comb / (5 + 1);
return x_extr;
}


int decodeY(int comb)
{
int y_extr = comb % (5 + 1);
return y_extr;
}



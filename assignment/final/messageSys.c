#include "messageSys.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int8_t encode(int8_t x, int8_t y) 
{
int8_t combine = (x << 8) | y;
return combine;
}


int8_t decodeX()
{
int y_mask = (1 << 8) - 1; // Create a mask with n lower bits set to 1
int x_extracted = combined_value >> 8;
return x_extracted;
}


int8_t decodeY()
{
int y_mask = (1 << 8) - 1; // Create a mask with n lower bits set to 1
int y_extracted = combined_value & y_mask;
return y_extracted;
}


int main(void) {
    printf(encode(4,8));
}
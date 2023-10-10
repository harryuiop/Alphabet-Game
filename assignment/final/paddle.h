#ifndef PADDLE_H
#define PADDLE_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    int x_pos1;
    int x_pos2;
} paddle_t;

void moveLeft();

void moveRight();

#endif /* PADDLE_H */
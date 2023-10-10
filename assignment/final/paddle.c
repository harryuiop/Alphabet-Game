#include "paddle.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void moveRight(paddle_t *player) {
    if (player->x_pos2 < 4) {
        display_clear();
        player->x_pos1++; 
        player->x_pos2++;
        display_pixel_set(player->x_pos1, 6, 1);
        display_pixel_set(player->x_pos2, 6, 1);
    }
}

void moveLeft(paddle_t *player) {
    if (player->x_pos1 > 0) {
        display_clear();
        player->x_pos1--; 
        player->x_pos2--;
        display_pixel_set(player->x_pos1, 6, 1);
        display_pixel_set(player->x_pos2, 6, 1);
    }
}
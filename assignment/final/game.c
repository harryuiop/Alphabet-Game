// 260 Assingment: Harry Ellis & Sean Gouw

#include <avr/io.h>
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "pio.h"
#include "led.h"
#include "ir_uart.h"
#include "paddle.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PACER_RATE 500
#define MESSAGE_RATE 10

typedef enum {
    SETUP,
    IN_PROGRESS,
    FINISHED
} game_state;
game_state state = SETUP;


int main (void)
{
    system_init ();
    navswitch_init ();
    ir_uart_init ();
    // led_init ();
    // led_set(LED1, 0);
    pacer_init (PACER_RATE);
    tinygl_init (PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);

    tinygl_text("PONG! PRESS TO PLAY");
    paddle_t player = {0,1};
    while (1)
    {
        pacer_wait();
        tinygl_update();
        navswitch_update();

        switch (state)
        {
            case SETUP:
                if (navswitch_push_event_p(NAVSWITCH_PUSH))
                {
                    tinygl_clear();
                    state = IN_PROGRESS;
                    display_pixel_set(player.x_pos1, 6, 1);
                    display_pixel_set(player.x_pos2, 6, 1);
                }

                tinygl_update();
                break;

            case IN_PROGRESS:
                if (navswitch_push_event_p(NAVSWITCH_WEST))
                {   
                    moveLeft(&player);
                }
                if (navswitch_push_event_p(NAVSWITCH_EAST))
                {
                    moveRight(&player);
                }
                
                break;

            case FINISHED:
                break;
        }
    }
}
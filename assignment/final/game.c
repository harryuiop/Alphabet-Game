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
#include "messageSys.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PACER_RATE 550
#define MESSAGE_RATE 10



typedef enum {
    SETUP,
    START_ROUND,
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
    tinygl_text("PRESS TO PLAY");


while (1) {
    pacer_wait();
    tinygl_update();
    navswitch_update();
    display_update();

    switch (state) {
        case SETUP:
            if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                state = START_ROUND;
                tinygl_clear();
                tinygl_text("HIT");
            }
            break;


        case START_ROUND:

            if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
                tinygl_text("HIT");
            }

            if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
                tinygl_text("STAY");
            }

            break;

        case FINISHED:
            break;
        }
    }
}


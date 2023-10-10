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


char* game_total = "0";
char* game_letter[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
int index = 0;

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
    led_init ();
    // led_set(LED1, 0);
    pacer_init (PACER_RATE);
    tinygl_init (PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text(game_total);


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
                tinygl_text(game_letter[index]);
            }
            break;

        case START_ROUND:
            if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
                index++;
                tinygl_text(game_letter[index]);
            }

            if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
                index--;
                tinygl_text(game_letter[index]);
            }

            if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                led_set(LED1, 0);
                ir_uart_putc(index);
                tinygl_clear();
            }

            if (ir_uart_read_ready_p()) {
                index = ir_uart_getc();
                tinygl_text(game_letter[index]);
            }
            break;

        case FINISHED:
            break;
        }
    }
}


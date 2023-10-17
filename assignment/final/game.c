/**  @file   game.c
     @author Harry Ellis & Sean Gouw
     @date   October 2023
     @brief  Main game loop and variables for game function
*/


#include <avr/io.h>
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "../fonts/font5x7_1.h"
#include "pio.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "led.h"
#include "logic.h"
#include "communication.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define PACER_RATE 550
#define MESSAGE_RATE 10


char* game_letter[] = {"A","B","C","D","E","F","G","H","I","J",
                        "K","L","M","N","O","P","Q","R","S","T",
                        "U","V","W","X","Y","Z"};


typedef enum {
    SETUP,
    START_ROUND,
    FINISHED
    } game_state;


game_state state = SETUP;


// CurrentIndex sets where in the array the players are, maxpush sets how many letters a player has the option of
int currentIndex = 0;
int maxpush = 3;


// Controls which player is allowed to use the navagation switch
int myturn = 1;


// Does an intial setup of the hardware components, then loops through the three game states continuously.
int main (void)
{
    system_init ();
    navswitch_init ();
    ir_uart_init ();
    led_init ();
    pacer_init (PACER_RATE);
    tinygl_init (PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text("PRESS TO START");


while (1) {
    pacer_wait();
    tinygl_update();
    navswitch_update();
    display_update();


    switch (state) {
        case SETUP:              
            setup_game();
            break;


        case START_ROUND:
            if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
                increment_letter(); 
            }

            if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
                decrement_letter();
            }

            if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                send_letter();
            }

            if (ir_uart_read_ready_p()) {
                receive_letter();
            }
                break;


        case FINISHED:
            if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
                reset_game(); 
            }
            break;
        }
    }
}

/**  @file   game.c
     @author Harry Ellis & Sean Gouw
     @date   October 2023
     @brief  Main game loop and variables for game function
*/


#include <avr/io.h>
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "pio.h"
#include "led.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "led.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define PACER_RATE 550
#define MESSAGE_RATE 10

#define WINNER_IR_MSG 'A'
#define RESET_GAME_MSG 'B'
#define SETUP_GAME_MSG 'S'


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

// Increments the letter on the LCD when the navagation swtich gets pushed north
void increment_letter(void) 
{
    if (currentIndex < maxpush) {
        if (myturn && currentIndex < 25) { 
            currentIndex++;
            tinygl_text(game_letter[currentIndex]);
        }
    }
}


// Decrements the letter on the LCD when the navagation swtich gets pushed south
void decrement_letter(void)
{
    if (myturn && currentIndex > maxpush - 2) {
        currentIndex--;
        tinygl_text(game_letter[currentIndex]);
    }
}


// Checks if a player is on the losing letter (default 'Z') and changes game state, else sends letter to other LCD using IR library
void send_letter(void)
{   
    if (myturn && currentIndex > maxpush - 3) {
        if (currentIndex != 25) { 
            led_set(LED1, 0);
            ir_uart_putc(currentIndex);
            tinygl_clear();
            myturn = 0;
        } else {
            tinygl_clear();
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            tinygl_text("LOSER");
            state = FINISHED;
            ir_uart_putc(WINNER_IR_MSG);
        }
    }
}


// Recives a letter from the other player using the IR Library and displays it on the LCD 
void receive_letter(void)
{
    currentIndex = ir_uart_getc();

    if (currentIndex == WINNER_IR_MSG) {
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        tinygl_text("WINNER");
        return;
    }

    if (currentIndex == RESET_GAME_MSG) {
        tinygl_clear();
        return;
    }


    tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
    tinygl_text(game_letter[currentIndex]);
    led_set(LED1, 1);
    maxpush = currentIndex + 3;
    myturn = 1;
}


// Gets called in the intial game setup after clicking inital navigation switch push down
// One player pushes navigation switch push down to start the game, this sends a message through IR to start other players game too
void setup_game(void)
{
    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
        maxpush = 3;
        currentIndex = 0;
        state = START_ROUND;
        tinygl_clear();
        tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
        tinygl_text(game_letter[currentIndex]);
        ir_uart_putc(SETUP_GAME_MSG);
    }

    if (ir_uart_read_ready_p()) {
        if (ir_uart_getc() == SETUP_GAME_MSG) {
            tinygl_clear();
            led_set(LED1, 0);
            state = START_ROUND;
            myturn = 0;
        }
    }
}


// Only called after a game is complete to the same job as setup_game() without needing an input to start again
void reset_game(void)
{
    maxpush = 3;
    currentIndex = 0;
    state = START_ROUND;
    tinygl_clear();
    tinygl_text(game_letter[currentIndex]);
    ir_uart_putc(RESET_GAME_MSG);
}


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

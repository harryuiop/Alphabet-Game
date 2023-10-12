// 260 Assignment: Harry Ellis & Sean Gouw

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
char* game_letter[] = {"A","B","C","D","E","F","G","H","I","J",
                        "K","L","M","N","O","P","Q","R","S","T",
                        "U","V","W","X","Y","Z"};


typedef enum {
    SETUP,
    START_ROUND,
    FINISHED
} game_state;
game_state state = SETUP;


int maxpush = 3;
int currentIndex = 0;
int myturn = 1;


void increment_letter(void) 
{
    if (currentIndex < maxpush) {
        if (myturn && currentIndex < 25) { 
            currentIndex++;
            tinygl_text(game_letter[currentIndex]);
        }
    }
}


void decrement_letter(void)
{
    if (myturn && currentIndex > maxpush - 2) {
        currentIndex--;
        tinygl_text(game_letter[currentIndex]);
    }
}


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
        }
    }
}


void receive_letter(void)
{
    currentIndex = ir_uart_getc();
    tinygl_text(game_letter[currentIndex]);
    led_set(LED1, 1);
    maxpush = currentIndex + 3;
    myturn = 1;
}


void setup_game(void)
{
    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
        maxpush = 3;
        currentIndex = 0;
        state = START_ROUND;
        tinygl_clear();
        tinygl_text(game_letter[currentIndex]);
        ir_uart_putc('S');
    }

    if (ir_uart_read_ready_p()) {
        if (ir_uart_getc() == 'S') {
            tinygl_clear();
            state = START_ROUND;
            myturn = 0;
        }
    }
}


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
    tinygl_text(game_total);


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
                tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
                maxpush = 3;
                currentIndex = 0;
                setup_game();
            }
            break;
        }
    }
}

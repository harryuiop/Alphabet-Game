/**  @file   communication.h
     @author Harry Ellis & Sean Gouw
     @date   October 2023
     @brief  Deals with IR communication between microcontrollers
*/


#include "communication.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "led.h"


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
            ir_uart_putc('A');
        }
    }
}


// Recives a letter from the other player using the IR Library and displays it on the LCD 
void receive_letter(void)
{
    currentIndex = ir_uart_getc();

    if (currentIndex == 'A') {
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        tinygl_text("WINNER");
        return;
    }

    if (currentIndex == 'B') {
        tinygl_clear();
        return;
    }

    tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
    tinygl_text(game_letter[currentIndex]);
    led_set(LED1, 1);
    maxpush = currentIndex + 3;
    myturn = 1;
}
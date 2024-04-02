#include "../lib/ev3dev.h"
#include <iostream>
#include <thread>
#include <chrono>

/** Maximum value */
#define BYTE_PIXEL         4    // 4 byte/pixel (32bit)
#define SCREEN_WIDTH     178    // pixel
#define SCREEN_HEIGHT    128    // pixel
#define SCREEN_SIZE      SCREEN_WIDTH * SCREEN_HEIGHT * BYTE_PIXEL    // 91136 byte
#define LINE_LENGTH      SCREEN_WIDTH * BYTE_PIXEL    // 712 byte

/** Frame buffer path **/
#define PATH_FB         "/dev/fb0"

/**  Color code (Used lower 24 bits) */
#define COLOR_BLACK      0x00000000
#define COLOR_WHITE      0x00ffffff
#define COLOR_RED        0x00ff0000
#define COLOR_GREEN      0x0000ff00
#define COLOR_BLUE       0x000000ff

using namespace ev3dev;

int main() {
    lcd ev3_lcd;
    
    // // 0 => black, 
    // unsigned char lcd_buffer = 0; // buffer to store the LCD display data
    // // Set the lcd_buffer to black color

    // ev3_lcd.fill(lcd_buffer); // fill the screen with black color

    int  start_row     =  1;
    int  str_col       =  1;
    int  max_pixel     =  5696;
    int  pos           = ((start_row - 1) * SCREEN_WIDTH + (str_col - 1)) * BYTE_PIXEL;

    // Fill the screen with blue color
    for (int i = 0; i <= max_pixel; i++) {
        ev3_lcd.fill(COLOR_BLUE, pos);
        pos += BYTE_PIXEL;
    }

    // Set the lcd_buffer to red color
    for (int i = 0; i <= max_pixel; i++) {
        ev3_lcd.fill(COLOR_RED, pos);
        pos += BYTE_PIXEL;
    }

    // Set the lcd_buffer to green color
    for (int i = 0; i <= max_pixel; i++) {
        ev3_lcd.fill(COLOR_GREEN, pos);
        pos += BYTE_PIXEL;
    }

    // Set the lcd_buffer to black color
    for (int i = 0; i <= max_pixel; i++) {
        ev3_lcd.fill(COLOR_BLACK, pos);
        pos += BYTE_PIXEL;
    }




    std::this_thread::sleep_for(std::chrono::seconds(5)); // sleep for 5 seconds

    return 0;
}
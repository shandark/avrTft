#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <registers.h>
#include <util/delay.h>
#include <tft/IDisplay.h>

#define SCK   PB5 // pin 13
#define SDA   PB3 // pin 11
#define CS    PB2 // pin 10 - low when start transmission
#define A0    PB1 // pin  9 - 0 = command / 1 = data
#define RESET 1   // 3v3
#define LED   1   // 3v3




int main(void)
{

    initSPI(SCK, SDA, CS, A0, RESET);
        
    initDisplay();

    for (int i = 1 ; i < 0x0FFF; i <<= 1)
    {
        clearScreen(i);
        _delay_ms(50);
    }
    
    clearScreen(0xFFFF);

    return 0;
}

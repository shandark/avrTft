#ifndef ILI9163_H
#define ILI9163_H

#include <tft/IDisplay.h>
#include <stdint.h>



void initSPI( uint8_t SCK, uint8_t SDA, uint8_t CS, uint8_t A0, uint8_t reset );

void sendCommandSPI( uint8_t byte );

void sendDataSPI( uint8_t byte );

void sendDataSPI_16( uint16_t byte );

void setAddress( uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2 );

void clearScreen( uint16_t col );

void initDisplay( void );

void setPxl( uint8_t x, uint8_t y, uint16_t col );


#endif

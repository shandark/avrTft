#ifndef IDISPLAY_H
#define IDISPLAY_H

#include <stdint.h>


/**
 * @brief Initialize basic SPI interface in AVR
 *
 * @param SCK AVR Pin number for Serial Clock
 * @param SDA AVR Pin number for Master Output Slave Input
 * @param CS AVR Pin number for Slave Select
 * @param A0 AVR Pin number for D/C
 * @param reset Not used
 * 
 */
extern void initSPI( uint8_t SCK, uint8_t SDA, uint8_t CS, uint8_t A0, uint8_t reset );


/**
 * @brief Initialize display register, perform soft reset and start
 *
 */
extern void initDisplay( void );


/**
 * @brief Clear display screen 
 *
 * @param col Set color 0x0001 for black, 0xFFFF for white
 */
extern void clearScreen( uint16_t col );

extern void setPxl( uint8_t x, uint8_t y, uint16_t col );


#endif

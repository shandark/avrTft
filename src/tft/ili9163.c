#include <tft/ili9163.h>

#include <registers.h>
#include <util/delay.h>
#include <avr/io.h>

static uint8_t g_SCK, g_SDA, g_CS, g_A0, g_Reset;


void initSPI( uint8_t SCK, uint8_t SDA, uint8_t CS, uint8_t A0, uint8_t reset ) 
{ 
    g_SCK = SCK;
    g_SDA = SDA;
    g_CS = CS;
    g_A0 = A0;
    g_Reset = reset;
    
    DDRB = (1 << SCK) | (1 << SDA) | (1 << CS) | (1 << A0);              
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0) | (0 << DORD) | (1 << CPOL) | (1 << CPHA);   
    SPSR |= (1 << SPI2X);
} 


void sendCommandSPI( uint8_t byte ) 
{    
    PORTB &= ~(1 << g_CS);
    PORTB &= ~(1 << g_A0);
    
    SPDR = byte;
    while( !( SPSR & (1 << SPIF) ) );     

    PORTB |= (1 << g_CS);
}     


void sendDataSPI( uint8_t byte ) 
{    
    PORTB &= ~(1 << g_CS);
    PORTB |= (1 << g_A0);
    
    SPDR = byte;
    while( !( SPSR & (1 << SPIF) ) );   

    PORTB |= (1 << g_CS);
}

void sendDataSPI_16( uint16_t byte ) 
{    
    PORTB &= ~(1 << g_CS);
    PORTB |= (1 << g_A0);
    
    SPDR = (uint8_t)( (byte >> 8) & 0x00FF );
    while( !( SPSR & (1 << SPIF) ) );    
    	
    SPDR = (uint8_t)( byte & 0x00FF );
    while( !( SPSR & (1 << SPIF) ) ); 

    PORTB |= (1 << g_CS);
}

void setAddress( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 )
{
    sendCommandSPI(SET_COLUMN_ADDRESS);
    sendDataSPI_16(x1);
    sendDataSPI_16(x2);

    sendCommandSPI(SET_PAGE_ADDRESS);
    sendDataSPI_16(y1);
    sendDataSPI_16(y2);

    sendCommandSPI(WRITE_MEMORY_START);
}

void clearScreen( uint16_t col )
{
    setAddress(0, 0, 128, 128);
    for (uint8_t y = 128; y > 0; y--)
        for (uint8_t x = 128; x > 0; x--)
            sendDataSPI_16(col);
}

void initDisplay( void )
{
    sendCommandSPI(SOFT_RESET);
    _delay_ms(10);
    
    sendCommandSPI(EXIT_SLEEP_MODE);
    _delay_ms(120); // ILI9163 doc page 104 - Sleep out
    
    sendCommandSPI(SET_PIXEL_FORMAT);
    sendDataSPI(0x05); // ILI9163 doc page 143 - 16 bits per pixel

    // Doc page 145
    sendCommandSPI(FRAME_RATE_CONTROL1);
    sendDataSPI(0x11); // DIVA = 17
    sendDataSPI(0x14); // VPA  = 20
    
    sendCommandSPI(DISPLAY_INVERSION);
    sendDataSPI(0x07); // NLA = 1, NLB = 1, NLC = 1
   
    sendCommandSPI(POWER_CONTROL1);
    sendDataSPI(0x0A); // VRH = 10:  GVDD = 4.30
    sendDataSPI(0x02); // VC = 2: VCI1 = 2.65
      
    sendCommandSPI(POWER_CONTROL2);
    sendDataSPI(0x02); // BT = 2: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 5xVCI1, VGL = -2xVCI1

    sendCommandSPI(VCOM_CONTROL1);
    sendDataSPI(0x50); // VMH = 80: VCOMH voltage = 4.5
    sendDataSPI(0x5B); // VML = 91: VCOML voltage = -0.225
    
    sendCommandSPI(VCOM_OFFSET_CONTROL);
    sendDataSPI(0x40); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML
    
    // Page 115 - Set maximum width
    sendCommandSPI(SET_COLUMN_ADDRESS);
    sendDataSPI(0x00); // XSH
    sendDataSPI(0x00); // XSL
    sendDataSPI(0x00); // XEH
    sendDataSPI(0x7F); // XEL (128 pixels x)
   
    // Page 118 - Set maximum height
    sendCommandSPI(SET_PAGE_ADDRESS);
    sendDataSPI(0x00); // YSH
    sendDataSPI(0x00); // YSL
    sendDataSPI(0x00); // YEH
    sendDataSPI(0x7F); // YEL (128 pixels y)
    
    // Page 135 - None set
    sendCommandSPI(SET_ADDRESS_MODE);
    sendDataSPI(0x0);

    sendCommandSPI(SET_DISPLAY_ON);
    sendCommandSPI(WRITE_MEMORY_START);
   
}

#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <registers.h>
#include <util/delay.h>
#include <tft/IDisplay.h>
#include <avr/pgmspace.h>

#define SCK   PB5 // pin 13
#define SDA   PB3 // pin 11
#define CS    PB2 // pin 10 - low when start transmission
#define A0    PB1 // pin  9 - 0 = command / 1 = data
#define RESET 1   // 3v3
#define LED   1   // 3v3

#define PI 3.14159265

static const uint16_t PROGMEM image1[] = {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xce79,0x9492,0x632d,0x4a69,0x4229,0x4229,0x4a69,0x6b4d,0x94b3,0xd69a,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0xd69a,0x6b8e,0x10a3,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18a3,0x7bef,0xdedb,0xf7de,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xad76,0x39c8,0x841,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x841,0x4229,0xbdf7,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef5d,0x52ab,0x0,0x0,0x0,0x0,0x1062,0x2125,0x4a49,0x73af,0x94b2,0xa534,0xa534,0x9cd3,0x7bf0,0x52ab,0x2946,0x10a3,0x0,0x0,0x0,0x0,0x630c,0xef5d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xb5b6,0x18e4,0x0,0x0,0x0,0x10a3,0x6b8e,0xad55,0xc638,0xdeda,0xdedb,0xc638,0xc638,0xc638,0xc618,0xd6ba,0xdeda,0xce79,0xb596,0x8410,0x2104,0x0,0x0,0x0,0x2945,0xbdf7,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cd3,0x0,0x0,0x0,0x1082,0x52ab,0xc658,0xce79,0xbdf7,0x9cf3,0x632d,0x4a49,0x52ab,0x6b6e,0x7bcf,0x6b6e,0x4a6a,0x5aec,0x9492,0xb5b6,0xce59,0xd6ba,0x73af,0x18c3,0x0,0x0,0x862,0xbdf7,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x94d3,0x841,0x0,0x0,0x2105,0xa534,0xd6ba,0xad76,0x52ab,0x9cf3,0xce59,0xef9d,0xffff,0xdf1c,0x738e,0x8410,0xffff,0xffff,0xf7be,0xd6ba,0xad55,0x632d,0x9cd3,0xdedb,0xb5b6,0x4229,0x0,0x0,0x841,0xa534,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xb596,0x0,0x0,0x0,0x39c7,0xce79,0xce59,0x8c72,0x6b6e,0xffff,0xffff,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8c51,0x6b4d,0xc618,0xce79,0x4a8a,0x21,0x0,0x1082,0xd6ba,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xe75c,0x18e4,0x0,0x0,0x39c7,0xc638,0xd6ba,0x8c51,0x7c10,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9d14,0x9cf3,0xd69a,0xce79,0x632d,0x20,0x0,0x2104,0xef7d,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0x4a6a,0x0,0x0,0x2125,0xce79,0xce59,0x4249,0xbdf7,0x4a8a,0xdf1c,0xffff,0xffff,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0x5b0c,0xc618,0x39e8,0xbdd7,0xce79,0x3a08,0x0,0x0,0x52cb,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xa534,0x0,0x0,0x1083,0xa555,0xce59,0x632d,0xdefb,0x632d,0xce59,0x6b4d,0xffff,0xffff,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0x52ab,0xb5b6,0x52ab,0xe73c,0x5aec,0xbdd7,0xc638,0x2125,0x0,0x841,0xce9a,0xffff,0xffff,0xffff,
0xffff,0xffff,0xf7be,0x31a7,0x0,0x0,0x5b0c,0xd6ba,0x8431,0xce59,0xffff,0xd6ba,0x528a,0xa555,0x6b6e,0xffff,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0x8c51,0xbdf7,0x73ae,0xef7d,0xffff,0xffff,0x52cb,0xce59,0x9cf3,0x841,0x0,0x4249,0xffff,0xffff,0xffff,
0xffff,0xffff,0xc659,0x21,0x0,0x18e4,0xce79,0xa534,0x7bcf,0xffff,0xffff,0xffff,0xbdf7,0x8c71,0x94b2,0xad55,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xa534,0x8c51,0x7bcf,0xad55,0xffff,0xffff,0xffff,0xbdd7,0x9cf3,0xce79,0x31a7,0x0,0x0,0xce59,0xffff,0xffff,
0xffff,0xffff,0x5aec,0x0,0x0,0x7bf0,0xce79,0x5acb,0xffff,0xffff,0xffff,0xffff,0xffff,0x73cf,0xb5b6,0x632d,0xe75d,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xce9a,0x630c,0xad55,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0x4a49,0xd6ba,0x9cd3,0x0,0x0,0x5aec,0xffff,0xffff,
0xffff,0xffff,0x842,0x0,0x10a3,0xb596,0xbdd7,0xa534,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x630c,0xb596,0x52ab,0xef5d,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0x632d,0xce59,0x6b4d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xc618,0x7bcf,0xd6ba,0x2125,0x0,0x2966,0xef9d,0xffff,
0xffff,0xb5d7,0x0,0x0,0x2986,0xce79,0x8c72,0xd6ba,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7de,0x6b6d,0xc638,0x73cf,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0x52ab,0xb5d6,0x4209,0xef5d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x630c,0xce79,0x6b6e,0x0,0x862,0xdefb,0xffff,
0xffff,0x7bf0,0x0,0x0,0x5b0c,0xdedb,0x52ab,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xbdf7,0x6b4d,0x9cd3,0x8c51,0xa555,0x73ae,0x7bf0,0x9cf4,0x8c92,0xa534,0x7c10,0xce9a,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x94d3,0xbdf7,0x9d14,0x21,0x0,0xbdf7,0xffff,
0xffff,0x528a,0x0,0x0,0x8c72,0xce99,0x5acb,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xa555,0x94b2,0x7bf0,0x8410,0xce79,0xce79,0xad55,0x8410,0xa555,0x8c71,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xbdf7,0xb5b6,0xad96,0x1082,0x0,0x8c71,0xffff,
0xffff,0x4208,0x0,0x21,0xad75,0xad75,0x632d,0xce9a,0xdedb,0xe73c,0xf79e,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x73af,0xce99,0xd69a,0x8c92,0x8431,0xce79,0xce59,0x4249,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf79e,0xe73c,0xdedb,0xce79,0x94b3,0xa535,0xbdd7,0x18e4,0x0,0x6b6e,0xffff,
0xf7de,0x39c8,0x0,0x21,0xbdf7,0xc618,0x9492,0x8410,0x73ae,0x632d,0x52ab,0x4249,0x39c7,0x39e8,0x4209,0x52ab,0x632d,0x52ab,0xce79,0x7bef,0x630c,0x6b4d,0x52ab,0xdedb,0x39e8,0x6b4d,0x52ab,0x4209,0x39e8,0x39c8,0x4249,0x52ab,0x632d,0x73ae,0x8410,0x9492,0xbdf7,0xbe17,0x2104,0x0,0x5aec,0xffff,
0xf7de,0x39e8,0x0,0x21,0xc618,0xad96,0x73ae,0x8410,0x8410,0x7c10,0x7bf0,0x8431,0x9492,0x9cd3,0xa514,0xa534,0xa514,0xad76,0xce59,0x634d,0x8430,0x8c51,0x2125,0xd6da,0xb596,0xa514,0xa514,0xa514,0x9cd3,0x9492,0x8431,0x7bf0,0x7c10,0x8410,0x8410,0x7bcf,0xa555,0xc618,0x2124,0x0,0x5aec,0xffff,
0xffff,0x4208,0x0,0x21,0xb5b6,0xa534,0x94d3,0xffff,0xffff,0xffff,0xffff,0xef7d,0xdedb,0xc638,0xb5b6,0xa514,0x8c72,0x4a8a,0xc638,0x9cf3,0x31c7,0x39e8,0x8c51,0xce59,0x4208,0x9492,0x9d14,0xb596,0xc638,0xdedb,0xef7d,0xffff,0xffff,0xffff,0xffff,0xce79,0xa534,0xbdf7,0x2104,0x0,0x6b6d,0xffff,
0xffff,0x4a8a,0x0,0x21,0x9cf3,0xbe18,0x6b6e,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xc618,0x6b6d,0xdedb,0xc618,0xb5d7,0xce79,0xad55,0x6b4d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xc618,0xb596,0xb5b6,0x18c3,0x0,0x8c51,0xffff,
0xffff,0x7bcf,0x0,0x0,0x73af,0xdedb,0x4a49,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x632d,0xce59,0x4229,0x8c72,0x94b2,0x4208,0x73af,0xbdf7,0x632d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xad55,0xb5d7,0xad55,0x841,0x0,0xb5d7,0xffff,
0xffff,0xb5b6,0x0,0x0,0x4209,0xd6ba,0x738e,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7de,0x632d,0xad76,0x5b0c,0xb596,0x6b8e,0x7bf0,0xffff,0xad96,0x738e,0xad75,0x6b6d,0xef7d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x73cf,0xc638,0x8c51,0x0,0x861,0xdefb,0xffff,
0xffff,0xffff,0x821,0x1082,0x2145,0xbdf7,0xad75,0xbdf7,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x6b8e,0xbdf7,0x632d,0xe73c,0xdefb,0x738e,0x8410,0xffff,0xffff,0xd6da,0x5aec,0xc638,0x5aec,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0x5acb,0xd69a,0x4a49,0x0,0x2965,0xef7d,0xffff,
0xffff,0xffff,0x5acb,0x1082,0x10a3,0x9cf3,0xc618,0x73cf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8c72,0x9cf3,0x7bf0,0xb596,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xc638,0x6b4d,0xb596,0x630c,0xdefb,0xffff,0xffff,0xffff,0xffff,0x6b8e,0xb596,0xc618,0x841,0x0,0x5acc,0xffff,0xffff,
0xffff,0xffff,0xc659,0x1082,0x1082,0x4249,0xd69a,0x73ae,0xc638,0xffff,0xffff,0xffff,0xffff,0x8c72,0x94d3,0x7bf0,0xad55,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xef5d,0x5acb,0xce59,0x630c,0xef9d,0xffff,0xffff,0xdefb,0x73ae,0xd6ba,0x5acb,0x0,0x0,0xc618,0xffff,0xffff,
0xffff,0xffff,0xf79e,0x31a7,0x1082,0x841,0x9cf4,0xd6ba,0x528a,0xef9e,0xffff,0xffff,0xbdf7,0x8410,0xa555,0x8c71,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0xd6ba,0x632d,0xb5b6,0x5aec,0xce9a,0xffff,0x7bf0,0xbe18,0xb5b6,0x18a3,0x0,0x39e8,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0x9cd3,0x841,0x1061,0x2946,0xc618,0xbdd7,0x8430,0xffff,0xc638,0x5aec,0xad55,0x5b0c,0xffff,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0x5aec,0xc638,0x6b8e,0x73ae,0x8c71,0xd69a,0x5acb,0x0,0x821,0xce79,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0x4208,0x861,0x862,0x6b4d,0xd69a,0x8431,0x8431,0x6b6e,0xc638,0x6b4d,0xffff,0xffff,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xe75c,0x632d,0xad96,0x8431,0xd6da,0x8c51,0x0,0x0,0x52ab,0xffdf,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xdf1b,0x18c3,0x841,0x862,0x73cf,0xd69a,0xad75,0xbdf7,0x4a49,0xef9e,0xffff,0xffff,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8c71,0x73ae,0xce79,0xa534,0x1082,0x0,0x1082,0xef9d,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0x9d14,0x21,0x861,0x862,0x8431,0xd69a,0xa514,0x6b4d,0xd6ba,0xffff,0xffff,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xe71c,0x8430,0x8431,0xdedb,0xa535,0x18c3,0x0,0x1062,0xc638,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20,0x0,0x862,0x630c,0xd69a,0xc618,0x7bf0,0x5aec,0xef7d,0xffff,0xffff,0xffff,0xdefb,0x738e,0x8410,0xffff,0xffff,0xffff,0xffff,0xffff,0x7bf0,0x6b4d,0xbdf7,0xd69a,0x8c72,0x1083,0x0,0x0,0x9cf3,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8431,0x821,0x0,0x21,0x4249,0xad75,0xd6ba,0xbdf7,0x5b0c,0x7bef,0xad76,0xc638,0xb5d7,0x6b8e,0x7bf0,0xd6ba,0xce59,0xb5d7,0x8451,0x52ab,0xa555,0xd6ba,0xbdd7,0x630c,0x0,0x0,0x862,0x9cf3,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xbdf7,0x1082,0x0,0x0,0x10a3,0x52ab,0xbdf7,0xd6ba,0xc638,0xb5b6,0xad75,0x8c92,0xa514,0xa534,0x8c92,0xa555,0xb5b6,0xc618,0xd69a,0xce59,0x6b4d,0x18c3,0x0,0x0,0x862,0xc618,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xd6ba,0x4208,0x21,0x0,0x0,0x841,0x4249,0x8c72,0xad75,0xbdd7,0xc638,0xce59,0xce59,0xc658,0xbdf7,0xad76,0x94d3,0x52cb,0x1082,0x0,0x0,0x21,0x4a8a,0xef7d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7de,0xb5d7,0x2125,0x0,0x0,0x0,0x0,0x1082,0x18e4,0x2945,0x2966,0x2966,0x2945,0x2104,0x1082,0x0,0x0,0x0,0x0,0x3186,0xc659,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xad75,0x4a49,0x2104,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x821,0x2125,0x528a,0xb5b6,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xe75d,0xd6ba,0xa514,0x73ae,0x52ab,0x4229,0x4a49,0x52ab,0x73af,0xa534,0xd6ba,0xef5d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
};


static const uint16_t PROGMEM image2[] = {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0xf7be,0xffff,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0xe73c,0xbdf7,0xad75,0x632d,0x6b2d,0x4229,0x630c,0x73af,0x630c,0xa534,0xbdf7,0xce79,0xf7de,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef7d,0xb5b6,0x6b6e,0x18e4,0x1082,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10a3,0x39e8,0x52ab,0x9cf4,0xef7d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xa534,0x4a69,0x841,0x0,0x0,0x0,0x0,0x0,0x841,0x0,0x18a3,0x21,0x0,0x20,0x0,0x0,0x21,0x18c3,0x39e8,0x8c92,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0xd69a,0x52aa,0x821,0x0,0x0,0x0,0x1062,0x31a7,0x52ab,0x7bcf,0x7c10,0xad55,0x94d3,0x9cf3,0x6b8e,0x4229,0x39e8,0x18a3,0x0,0x0,0x0,0x18c3,0x5b0c,0xbe18,0xf7df,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8c92,0x3186,0x0,0x0,0x21,0x2104,0x52ab,0xa534,0xdefb,0xce79,0xc638,0xc618,0x94b2,0xbdd7,0xb5b6,0xbdf7,0xdedb,0xbdf7,0x9d14,0x73af,0x2945,0x0,0x0,0x0,0x39e7,0xc618,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef7d,0xad76,0x862,0x0,0x0,0x10a3,0x73af,0xbdd7,0xdefb,0xa555,0xad75,0x8431,0x8431,0x73ae,0xad55,0x9cf3,0xb5b6,0x8451,0x52ab,0x9492,0xc638,0xc638,0xb596,0x7bcf,0x2124,0x0,0x0,0x1,0x8c92,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7bcf,0x2104,0x0,0x21,0x39c7,0x8c92,0xd69a,0xb596,0x634d,0x8c71,0xad55,0x630d,0xce59,0xffff,0xffff,0xffff,0xf79e,0xffff,0xdedb,0x7bcf,0x6b6e,0x9d14,0xbdf7,0xce59,0x8c72,0x31a7,0x21,0x0,0x3186,0xbdf7,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xf7be,0xb5b6,0x0,0x0,0x0,0x632d,0xbe18,0xb5b6,0x8451,0x9d14,0xd6db,0xe71c,0x6b8e,0xa514,0xce7a,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xbdf7,0x6b4d,0x7c10,0x6b6d,0x7bcf,0xce79,0xb5b6,0x4a6a,0x0,0x0,0x10a3,0x9d14,0xffdf,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xad75,0x3186,0x0,0x841,0x4a8a,0xbdd7,0xbdf7,0x5aec,0xc659,0xf7de,0xffff,0xffdf,0x7bcf,0x8430,0x9cf3,0xffff,0xffff,0xffff,0xffff,0xffff,0xe71c,0x8410,0x6b8e,0xd6ba,0xffff,0x9cf3,0x6b8e,0xad55,0xbdd7,0x6b4d,0x21,0x0,0x39c7,0xdefb,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xe73c,0x4229,0x0,0x0,0x39e8,0xbdd7,0xad55,0xad75,0xce59,0xffff,0xffff,0xffff,0xffff,0xad55,0xa514,0x6b6e,0xf79e,0xffff,0xffff,0xffff,0xf7be,0xbdf7,0x6b4d,0x8c51,0xffff,0xffff,0xffff,0xdefb,0xad76,0xbdf7,0xc618,0x4208,0x0,0x0,0x634d,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0x8c51,0x10a3,0x0,0x2965,0x94d3,0xce79,0x632d,0xdefb,0xffdf,0xffff,0xffff,0xffff,0xffff,0xb5b6,0x8431,0x94b2,0xe71c,0xffff,0xffff,0xffff,0xe75c,0x8c51,0x6b6e,0xc638,0xffff,0xffff,0xffff,0xffdf,0xce79,0x632d,0xb596,0xa514,0x2104,0x0,0x1062,0xad75,0xffff,0xffff,0xffff,
0xffff,0xffff,0xe73c,0x31a7,0x0,0x0,0x8c71,0xc618,0x7bf0,0xa534,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xdefb,0xa514,0x73af,0xce79,0xffff,0xffff,0xf7de,0xc638,0x6b6e,0x8430,0xf7de,0xffff,0xffff,0xffff,0xffff,0xffff,0xd69a,0x8431,0xd6ba,0x8430,0x862,0x0,0x52cb,0xf79e,0xffff,0xffff,
0xffff,0xffff,0x94d3,0x10a3,0x0,0x3186,0xc658,0xad75,0x630c,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf79e,0x6b6e,0xa514,0xbdf7,0xffff,0xffff,0xef5d,0x9492,0x6b6d,0xb5d7,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8451,0xa514,0xbdf7,0x31a7,0x0,0x10a3,0xd6ba,0xffff,0xffff,
0xffff,0xf7be,0x4249,0x20,0x0,0x9492,0xc658,0xa554,0x8c51,0xb5b6,0xdefb,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x8c72,0x8c51,0xffff,0xffdf,0xce79,0x6b6e,0x7bf0,0xf79e,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7de,0xe73c,0x8431,0x9cf3,0xe71b,0x6b6e,0x0,0x0,0x73ae,0xf7be,0xffff,
0xffff,0xbe18,0x2965,0x0,0x2104,0xbdf7,0xa534,0x73cf,0x73cf,0x6b6e,0x738e,0xa555,0xf7be,0xffff,0xffff,0xffff,0xffff,0xbdd7,0x94b2,0x73cf,0xef9d,0xf79e,0x9cf3,0x6b4d,0xad75,0xffff,0xffff,0xffff,0xffff,0xffff,0xc638,0x6b6e,0x94b2,0x9492,0x8430,0xa514,0xbdd7,0x2966,0x0,0x2965,0xf79e,0xffff,
0xffff,0xa535,0x1082,0x0,0x5aec,0xbdf7,0x94d3,0xb5d7,0xd6da,0x94b2,0x738e,0x52aa,0x4a6a,0x8c72,0xef7d,0xffff,0xffff,0xd69a,0x7bf0,0x9cd3,0xb5d7,0xce79,0x738e,0x7bcf,0xef5d,0xffff,0xffff,0xf7de,0xc638,0x9cf3,0x8430,0xb5b6,0x8c71,0x632d,0xc618,0x8451,0xd6ba,0x4a69,0x0,0x18c3,0xce79,0xffff,
0xffff,0x9cd3,0x0,0x821,0x6b8e,0xd6ba,0x5b0c,0xe71c,0xffff,0xffff,0xdefb,0x9d14,0x7bef,0x4a8a,0x39c8,0x7bf0,0xe71c,0xffff,0x94b2,0x8410,0x7c10,0x9492,0x6b4d,0x9cf3,0xffff,0xffff,0xf7de,0x8c51,0x738e,0x9cf3,0x73af,0x8c72,0xbe18,0xffff,0xf7de,0x73af,0xc658,0x73cf,0x0,0x0,0xb596,0xffff,
0xffff,0x528a,0x0,0x18a3,0x8c51,0xb5b6,0x6b6e,0xffff,0xffff,0xffff,0xffff,0xffde,0xce9a,0xad75,0x9cd3,0x6b6e,0x5acb,0x7bf0,0x6b8e,0xce59,0xa534,0xc658,0xb5b6,0x94d3,0xad55,0x9492,0x7bef,0x8431,0xb596,0x7bf0,0xa534,0xf7be,0xffff,0xffff,0xffff,0x8c92,0x9cf3,0x9d14,0x841,0x0,0x6b4d,0xffff,
0xef9d,0x632d,0x1062,0x18e4,0xb596,0x9d14,0xc618,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0xd6ba,0xad96,0xa534,0xa514,0x8c51,0xad55,0xbdd7,0xa514,0x8c51,0xb5b6,0xa534,0x7bef,0xad75,0x8430,0x6b8e,0xb5d7,0xef5d,0xffff,0xffff,0xffff,0xffff,0xffff,0xbdf7,0xad76,0xb5d7,0x18c3,0x0,0x8410,0xffff,
0xffff,0x630c,0x0,0x39c7,0xb5b6,0xb5b6,0xbdf7,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef7d,0xbdd7,0x7bf0,0x8410,0xce59,0x6b4d,0x632d,0x6b4d,0x9492,0xdeda,0x8431,0x94d3,0x94d3,0xf79e,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xa514,0x8430,0xb5b6,0x10a3,0x0,0x52aa,0xffff,
0xf7be,0x39e8,0x1082,0x2125,0xbdf7,0x8c51,0xbdf7,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xce9a,0x8451,0xc618,0x6b6d,0x8410,0x7bcf,0x4208,0xdedb,0x39c8,0xdefb,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xbe18,0x9d14,0xbdd7,0x2125,0x0,0x5aec,0xffff,
0xef7d,0x5aec,0x0,0x18c4,0xbdf7,0x8c92,0xc659,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf79e,0xdefb,0x73af,0x8430,0xd69a,0x8c51,0x31a7,0x52aa,0x8410,0xd6ba,0x6b4d,0x6b4d,0x9492,0xe75c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xce59,0x73ae,0xce59,0x1062,0x0,0x632d,0xf79e,
0xffff,0x52ab,0x841,0x1082,0xa514,0xa514,0x8431,0xffff,0xffff,0xffff,0xffff,0xffff,0xe75c,0x9492,0x94b2,0x8c51,0x8c71,0x9cd3,0x5aec,0xbdd7,0xb5d7,0xbe18,0xce59,0x6b4d,0x9cd3,0x9cf3,0x632d,0x39e8,0x738e,0xdedb,0xffff,0xffff,0xffff,0xffff,0xffff,0x9d14,0xa555,0xa534,0x18e4,0x20,0x6b4d,0xffff,
0xffff,0xa514,0x20,0x1082,0x9492,0xc638,0x94b2,0xffdf,0xffff,0xffdf,0xce79,0xa514,0x8431,0x94b2,0xad55,0x632d,0x9cf4,0xb596,0x7bf0,0x8c71,0x6b4d,0x8431,0x9492,0x632d,0xad75,0x9cf3,0xad55,0x9cd3,0x5aec,0x3a08,0x73ae,0xc638,0xe75c,0xf79e,0xef9e,0xb596,0xbdd7,0x9cf3,0x862,0x0,0xad75,0xffff,
0xffff,0xad55,0x1062,0x821,0x632d,0xc638,0x7bf0,0xe75d,0xce9a,0x94b3,0x8431,0x9cf3,0x8410,0x8c51,0x9d14,0xce59,0xffff,0xce79,0x6b6d,0x7bf0,0xdefb,0x7c10,0xb5b6,0x9d14,0xffff,0xe75c,0xce79,0xc638,0xb5b6,0x8c51,0x630c,0x5aec,0x8410,0xb596,0xa555,0xa535,0xbdd7,0x73af,0x0,0x18e3,0xdedb,0xffff,
0xffff,0xdefb,0x18e4,0x841,0x39c7,0xc618,0x9d14,0x738e,0x94b2,0x7bf0,0x9cf3,0x8430,0x9492,0xdf1c,0xffff,0xffff,0xef7d,0x94b2,0x6b4d,0xb5b6,0xffff,0xb5b6,0x73cf,0x7c10,0xf7de,0xffff,0xffff,0xf7de,0xf7be,0xf79e,0xc638,0x8c51,0x73ae,0x7bcf,0x94b2,0xb596,0xc618,0x31a7,0x0,0x52ab,0xe71c,0xffff,
0xffff,0xffdf,0x6b6d,0x0,0x1082,0x9d14,0xc638,0xbe17,0x73af,0x7c10,0xbdd7,0xd6ba,0xffff,0xffff,0xffff,0xffdf,0xd69a,0x738e,0x7bf0,0xef7d,0xffff,0xf7be,0x8430,0x8c51,0x94d3,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xd69a,0x8c51,0x8c92,0xc638,0x9cd3,0x1082,0x821,0x7bcf,0xffdf,0xffff,
0xffff,0xffff,0xa514,0x821,0x0,0x4a8a,0xbdf7,0x8431,0x6b4d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef7d,0x9cf3,0x6b4d,0xad55,0xffff,0xffff,0xffff,0xa514,0xb596,0x6b4d,0xef9d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xce79,0xa514,0xc658,0x4a6a,0x0,0x2125,0xad96,0xffff,0xffff,
0xffff,0xffff,0xef9d,0x52aa,0x0,0x18c3,0xad75,0xc638,0xa514,0xf79e,0xffff,0xffff,0xffff,0xffff,0xffff,0xd6da,0x73ae,0x73cf,0xe73c,0xffff,0xffff,0xffff,0xd6db,0x7c10,0x9cf3,0xce79,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xdefb,0xb5b6,0xb596,0xa534,0x10a3,0x0,0x52ab,0xef9d,0xffff,0xffff,
0xffff,0xffff,0xffff,0x9cf3,0x0,0x0,0x4a8a,0xc638,0x9492,0x5b0c,0xffdf,0xffff,0xffff,0xffff,0xef9e,0xa555,0x6b4d,0x9d14,0xffff,0xffff,0xffff,0xffff,0xffff,0x73af,0x94d3,0x94b3,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9d14,0xb5b6,0xbdf7,0x4a69,0x0,0x2945,0xa535,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xf79e,0x4249,0x0,0x21,0x6b4d,0xce79,0x8c92,0xb5b6,0xffff,0xffff,0xffff,0xdefb,0x7bcf,0x73af,0xdefb,0xffff,0xffff,0xffff,0xffff,0xffff,0xc659,0x8451,0x632d,0xf79e,0xffff,0xffff,0xffff,0xffff,0xb5d7,0x7c10,0xc618,0x8c71,0x21,0x0,0x7c10,0xef7d,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xd6ba,0x39c7,0x0,0x18e4,0xa534,0xc638,0x8410,0xa534,0xdf1c,0xf79e,0xb596,0x6b4d,0x94b2,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8c92,0x94b2,0x94d3,0xffff,0xffff,0xf7be,0x73af,0x8c51,0xc618,0x8c51,0x2965,0x0,0x4a49,0xdefb,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xf7df,0x8c92,0x0,0x0,0x1083,0x8c51,0xc618,0xb5b6,0xb5d7,0xbdf7,0x7bcf,0x738e,0xd69a,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8c92,0xbdd7,0x9492,0xef7d,0xc659,0x9d14,0x8c71,0xce59,0xa555,0x10a3,0x0,0x18c3,0xc638,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x94b2,0x18c3,0x0,0x3166,0x9492,0xbdf7,0xad75,0x9492,0x8430,0x8451,0xef5d,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xd69a,0x6b8e,0x8451,0x738e,0x7bf0,0xc618,0xce59,0x6b6e,0x2104,0x0,0x2104,0x7bf0,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xe73c,0x738e,0x1082,0x0,0x841,0x52aa,0xad75,0xce79,0xbdf7,0x94d3,0xad76,0xbe18,0xbdf7,0xdedb,0xffff,0xbe18,0xe71c,0xb5b6,0x9d14,0x6b8e,0x9cf3,0xad55,0xc638,0xb5b6,0x5aec,0x21,0x0,0x841,0xa555,0xef7d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xbdf7,0x4208,0x0,0x0,0x18e4,0x52ab,0xa534,0xce59,0xb5b6,0xb596,0xad75,0x6b4d,0x7bf0,0x7bf0,0x8410,0xa514,0x94b2,0xc638,0xce79,0xbdd7,0x6b6d,0x2104,0x0,0x0,0x4228,0xa514,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0xc659,0x6b4d,0x18e4,0x0,0x21,0x18c3,0x4228,0x8431,0xad76,0xb5b6,0xce79,0xce9a,0xce59,0xce79,0xbdf7,0xb596,0x8431,0x5acb,0x18c3,0x0,0x0,0x0,0x5acb,0xdefb,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xe75c,0x94b3,0x4208,0x18c3,0x20,0x0,0x0,0x18a3,0x39e8,0x3187,0x4208,0x39e8,0x2966,0x31a6,0x821,0x20,0x0,0x0,0x1062,0x5aec,0xa555,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef7d,0xa514,0x630c,0x4a49,0x18c3,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1062,0x2125,0x6b4d,0xb5b7,0xf79e,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7de,0xd6ba,0xce79,0xa534,0x5b0c,0x52ab,0x528a,0x39e8,0x7bcf,0x630c,0xad75,0xbe18,0xef7d,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef7d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
};


int main(void)
{

    initSPI(SCK, SDA, CS, A0, RESET);
        
    initDisplay();
    
    clearScreen(0xffff);
    
    int rad = 0;
    double y = 0;
    
    
    while (1)
    {
      

        for (uint16_t j = 0; j < 128; j++)
        {
            rad += 10;
            y = 30*sin(rad* PI / 180);
            
            setPxl(j,y+128/2,0x0);
            
            _delay_ms(50);
        }
                
        clearScreen(0xffff);
        
        
        for (uint8_t time = 0; time < 40; time++)
        {
            
            for (uint16_t i = 0; i < 42; i++)
                for (uint16_t j = 0; j < 42; j++)
                {
                    setPxl(j+80,i+10,pgm_read_word(&(image1[i*42+j])));
                }
                
                    for (uint16_t i = 0; i < 42; i++)
                for (uint16_t j = 0; j < 42; j++)
                {
                    setPxl(j+80,i+10,pgm_read_word(&(image2[i*42+j])));
                }
            
        }
        
        clearScreen(0xFFFF);
        
        
    }
    
    

    return 0;
}

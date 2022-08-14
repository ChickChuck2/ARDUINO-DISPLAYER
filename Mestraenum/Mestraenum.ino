﻿#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);    //Low spped I2C
const uint8_t rook_bitmap[] PROGMEM = {
0x00, // 00000000
0x55, // 01010101
0x7f, // 01111111
0x3e, // 00111110
0x3e, // 00111110
0x3e, // 00111110
0x3e, // 00111110
0x7f // 01111111
};

void setup(void) {
	u8g2.begin();

	u8g2.clearBuffer();                   // clear the internal memory
	u8g2.setFont(u8g2_font_ncenB08_tr);   // choose a suitable font
	u8g2.drawStr(0, 10, "MERDA");    // write something to the internal memory
	u8g2.sendBuffer();
}


void loop(void) {
}
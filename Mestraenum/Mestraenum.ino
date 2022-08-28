#include <U8g2lib.h>
#include <ezButton.h>




U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);    //Low spped I2C


const int LED_GREEN = 4;
const int LED_RED = 3;
const int BUZZER = 2;

bool startedGame = false;
bool beenstarted = false;

ezButton BUTTON_UP(13);
ezButton BUTTON_DOWN(12);
ezButton BUTTON_LEFT(11);
ezButton BUTTON_RIGHT(10);

void setup(void) {
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);

	u8g2.begin();
	u8g2.setFont(u8g2_font_ncenB08_tr);
	u8g2.clearBuffer();
	u8g2.drawStr(0, 10, "Welcome do the");
	u8g2.drawStr(0, 20, "CUMZONE");
	u8g2.drawStr(0, 30, "Press UP to continue");
	u8g2.sendBuffer();
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, LOW);

}

void loop(void) {
	BUTTON_UP.loop();
	//BUTTON_DOWN.loop();
	//BUTTON_LEFT.loop();
	//BUTTON_RIGHT.loop();

	if (BUTTON_UP.isPressed()) {
		digitalWrite(LED_RED, LOW);
		digitalWrite(LED_GREEN, HIGH);
		startedGame = true;
	}
	if(startedGame)
		thecum();
}

void setonLineChange(uint8_t x, uint8_t y, const char* cwer) {
	u8g2.clearBuffer();
	u8g2.drawStr(x, y, cwer);
	u8g2.sendBuffer();
}

void thecum() {
	startedGame = false;
	u8g2.clearBuffer();
	u8g2.drawStr(0,10, "Iniciando jogos");
	u8g2.sendBuffer();
	delay(1000);
	u8g2.clearBuffer();
	u8g2.drawStr(0,10,"UwU");
	u8g2.sendBuffer();
	delay(700);
	setonLineChange(0, 10, "Carregando...");
	for (int i = 300; i < 500; i++) {
		tone(BUZZER, i, 20);
		delay(20);
	}
	setonLineChange(0, 10, "Bem-vindo, FILHA DA PUTA");
	delay(1000);
}
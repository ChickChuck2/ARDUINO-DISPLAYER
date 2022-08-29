#include <U8g2lib.h>
#include <ezButton.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);    //Low spped I2C


const int LED_GREEN = 4;
const int LED_RED = 3;
const int BUZZER = 2;

bool startedGame = false;
bool beenstarted = false;
bool change = false;
int loaded = 0;

int X = 0;
int Y = 10;
int increace = 1;
int MAXincreace = 30;

int biteX;
int biteY;

int randPOSX = random(10,114);
int randPOSY = random(10,54);

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
	u8g2.drawStr(0, 30, "Press UP to Play");
  u8g2.drawStr(0,40, "Or DOWN to Settings");
	u8g2.sendBuffer();
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, LOW);

}

bool Settings = false;
bool RandomOnEat = true;
bool StartSettings = false;
void loop(void) {
	BUTTON_UP.loop();
	BUTTON_DOWN.loop();
	BUTTON_LEFT.loop();
	BUTTON_RIGHT.loop();

	if (BUTTON_UP.isPressed() && !Settings && !startedGame && increace < MAXincreace) {
		digitalWrite(LED_RED, LOW);
		digitalWrite(LED_GREEN, HIGH);
		startedGame = true;
	}
  if(BUTTON_DOWN.isPressed() && !startedGame && increace < MAXincreace)
    Settings = true;

	if(startedGame){
    if(loaded == 0){

      loaded++;
      thecum();
    }
    game();
  }
  if(Settings)
    if(BUTTON_DOWN.isReleased())
      StartSettings = true;
  
  if(StartSettings)
    settingsMenu();
}
int former = 0;
void settingsMenu() {
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "UP");
  u8g2.drawStr(0, 20, " Points random on eat");
  u8g2.drawStr(0, 30, "DOWN");
  // ...
  u8g2.drawStr(0,50,"LEFT");
  // ...
  if(BUTTON_UP.isPressed()) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    RandomOnEat = true;
    Settings = false;
    StartSettings = false;
    startedGame = true;
  }
  if(BUTTON_DOWN.isPressed()) {
    if(MAXincreace == 30)
      MAXincreace = 90000;
    else
      MAXincreace = 30;
    //digitalWrite(LED_RED, LOW);
    //digitalWrite(LED_GREEN, HIGH);
    //Settings = false;
    //StartSettings = false;
    //startedGame = true;
  }
  if(BUTTON_LEFT.isPressed()) {
    if(former == 0){
      former++;
    }
    //else if(former == 1){
      //former++;
    //}
    //else if(former == 2){
    //  former++;
    //}
    else
      former = 0;
  }

  if(MAXincreace == 30)
    u8g2.drawStr(0,40, " Ball size 30");
  else
    u8g2.drawStr(0,40, " Ball size infi");
  
  if(former == 0)
    u8g2.drawStr(0,60, " Format: Circle");
  else
    u8g2.drawStr(0,60, " Format: Square");
  //if(former == 2)
    //u8g2.drawStr(0,60, " Format: Triangle");

  

  u8g2.sendBuffer();
}

void ChangePosition() {
  if(BUTTON_UP.getState() == 1 && increace <= MAXincreace)
    Y++;
  if(BUTTON_DOWN.getState() == 1 && increace <= MAXincreace)
    Y--;
  if(BUTTON_LEFT.getState() == 1 && increace <= MAXincreace)
    X++;
  if(BUTTON_RIGHT.getState() == 1 && increace <= MAXincreace)
    X--;
}
void VerifyBorder() {
  if(X >= 124)
    X--;
  else if(X <= 0)
    X++;
  if(Y >= 64)
    Y--;
  else if(Y <= 0)
    Y++;
}

void RandomBallPOS() {
  randPOSX = random(10,114);
  randPOSY = random(10,54);
}
void game() {
  VerifyBorder();
  u8g2.clearBuffer();

  if(change && RandomOnEat){
    RandomBallPOS();
    increace++;
    change = false;
  }

  if(RandomOnEat){
    u8g2.drawPixel(randPOSX, randPOSY);
    if(abs(X - randPOSX) <= increace && abs(Y - randPOSY) <= increace){
      change = true;
      tone(BUZZER, 10 * increace, 500);
    }
  }
  PlayerMove();
  u8g2.sendBuffer();

  if(increace >= MAXincreace){
    startedGame = false;
    u8g2.clearBuffer();
    u8g2.drawStr(10, 10, "parabens");
    u8g2.sendBuffer();
    delay(1000);
    u8g2.drawStr(10,30,"Voce perdeu seu");
    u8g2.sendBuffer();
    delay(1000);
    u8g2.drawStr(62,60,"TEMPO.");
    u8g2.sendBuffer();
    delay(2000);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  }
  ChangePosition();
}
void PlayerMove() {
  if(former == 0)
    u8g2.drawCircle(X, Y, increace);
  else if(former == 1)
    u8g2.drawBox(X, Y, increace, increace);
  //else if(former == 2)
    //u8g2.drawTriangle(X, Y, increace, increace, increace, increace);
}
void setonLineChange(uint8_t x, uint8_t y, const char* cwer) {
	u8g2.clearBuffer();
	u8g2.drawStr(x, y, cwer);
	u8g2.sendBuffer();
}
void drawOnCube(uint8_t x, uint8_t y, uint8_t w, uint8_t h){
  u8g2.clearBuffer();
  u8g2.drawBox(x, y, w, h);
}

void thecum() {
	u8g2.clearBuffer();
	u8g2.drawStr(0,10, "Iniciando jogos");
	u8g2.sendBuffer();
	delay(1000);
	u8g2.clearBuffer();
	u8g2.drawStr(0,10,"UwU");
	u8g2.sendBuffer();
	delay(700);
	setonLineChange(0, 10, "Carregando...");
	for (int i = 300; i < 400; i++) {
		tone(BUZZER, i, 20);
		delay(20);
	}
	setonLineChange(0, 10, "Bem-vindo");
  u8g2.drawStr(0, 20, "FILHA DA PUTA");
	delay(3000);
}
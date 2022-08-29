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
bool RandomOnLoad = false;
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

void settingsMenu() {
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "UP");
  u8g2.drawStr(0, 20, " Points random on eat");
  u8g2.drawStr(0, 30, "DOWN");
  u8g2.drawStr(0, 40, " Points Created on Start game");
  u8g2.drawStr(0, 50, "LEFT");
  u8g2.drawStr(0, 60, " Remove max ball size");
  u8g2.sendBuffer();
  if(BUTTON_UP.isPressed()) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    RandomOnEat = true;
    Settings = false;
    StartSettings = false;
    startedGame = true;
  }
  if(BUTTON_DOWN.isPressed()) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    RandomOnLoad = true;
    RandomOnEat = false;
    Settings = false;
    StartSettings = false;
    startedGame = true;
  }
  if(BUTTON_LEFT.isPressed()) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    MAXincreace = 90000;
    Settings = false;
    StartSettings = false;
    startedGame = true;
  }
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

int randX[10];
int randY[10];
void RandomBallPOS() {
  randPOSX = random(10,114);
  randPOSY = random(10,54);
}
int pointsCount = 0;
int eated = 0;
void game() {
  VerifyBorder();
  u8g2.clearBuffer();

  if(change && RandomOnEat){
    RandomBallPOS();
    increace++;
    change = false;
  }
  else if(RandomOnLoad && pointsCount <= 10){
    for(int i = 0; i <= 10; i++){
      randX[i] = random(10,114);
      randY[i] = random(10,54);
    }
  }
  for(int i = eated; i <= 10; i++)
    u8g2.drawPixel(randX[i], randY[i]);

  if(RandomOnEat){
    u8g2.drawPixel(randPOSX, randPOSY);
    if(abs(X - randPOSX) <= increace){
      change = true;
      tone(BUZZER, 5 * increace, 500);
    }
  }
  else{
    for(int i = eated; i <= 10; i++){
      if(abs(X - randX[i]) <= increace && abs(Y - randY[i] <= increace)) {
        tone(BUZZER, 5 * increace, 500);
        randX[i] = 0;
        randY[i] = 0;
        eated++;
        increace++;
      }
    }
  }
  
  PlayerMove();
  u8g2.sendBuffer();

  if(increace >= MAXincreace){
    startedGame = false;
    u8g2.clearBuffer();
    u8g2.drawStr(10, 10, "Voce acaba de zerar");
    u8g2.drawStr(10,20,"o melhor jogo");
    u8g2.drawStr(20,30,"Do mundo");
    u8g2.sendBuffer();
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  }
  ChangePosition();
}
void PlayerMove() {
  u8g2.drawCircle(X, Y, increace);
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
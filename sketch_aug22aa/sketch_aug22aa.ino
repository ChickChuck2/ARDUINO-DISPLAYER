#include <U8g2lib.h>
#include <ezButton.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);    //Low spped I2C


const int LED_GREEN = 4;
const int LED_RED = 3;
const int BUZZER = 2;

#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin

#define sad_width 9
#define sad_height 8
static unsigned char sad_bits[] = {
   0x7c, 0x00, 0x82, 0x00, 0x29, 0x01, 0x01, 0x01,
   0x39, 0x01, 0x45, 0x01, 0x82, 0x00, 0x7c, 0x00
};

#define happy_width 9
#define happy_height 8
static unsigned char happy_bits[] = {
   0x7c, 0x00, 0x82, 0x00, 0x29, 0x01, 0x01, 0x01,
   0x45, 0x01, 0x39, 0x01, 0x82, 0x00, 0x7c, 0x00
};

int xValue = 0;
int yValue = 0;

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

//ezButton BUTTON_UP(13);
//ezButton BUTTON_DOWN(12);
//ezButton BUTTON_LEFT(11);
//ezButton BUTTON_RIGHT(10);
ezButton BUTTON_VR(5);

void setup(void) {
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	u8g2.begin();
	u8g2.setFont(u8g2_font_ncenB08_tr);
  MenuStarter();
}
void MenuStarter() {
	u8g2.clearBuffer();
	u8g2.drawStr(0, 10, "Welcome do the");
	u8g2.drawStr(0, 20, "CUMZONE");
	u8g2.drawStr(0, 30, "Press VRB to Play");
  u8g2.drawStr(0,40, "Or DOWN to Settings");
	u8g2.sendBuffer();
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, LOW);
}

bool Settings = false;
bool RandomOnEat = true;
bool StartSettings = false;
bool loosed = false;
void loop(void) {
  if(!loosed) {
    xValue = map(analogRead(VRX_PIN), 0, 1023, -3, 4);
    yValue = map(analogRead(VRY_PIN), 0, 1023, -3, 4);

    //BUTTON_UP.loop();
    //BUTTON_DOWN.loop();
    //BUTTON_LEFT.loop();
    //BUTTON_RIGHT.loop();
    BUTTON_VR.loop();

    //if (BUTTON_UP.isPressed() && !Settings && !startedGame && increace < MAXincreace) {
    //	digitalWrite(LED_RED, LOW);
    //	digitalWrite(LED_GREEN, HIGH);
    //	startedGame = true;
    //}
    if(BUTTON_VR.isPressed() && !Settings && !startedGame && increace < MAXincreace) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      startedGame = true;
    }
    if(yValue == 1 && !startedGame && increace < MAXincreace && StartSettings == false)
      Settings = true;

    if(startedGame){
      if(loaded == 0){
        loaded++;
        thecum();
      }
      game();
    }
    if(Settings){
      u8g2.clearBuffer();
      u8g2.drawStr(0,10,"Nice, agora solta.");
      u8g2.sendBuffer();
      if(yValue == 0){
        StartSettings = true;
        Settings = false;
      }
    }
    if(StartSettings)
      settingsMenu();
  }
}

int IAX = random(0,133);
int IAY = random(10,60);
int IASIZE = random(increace + 1 ,increace + 2);
bool eated = false;

int former = 0;
int indexSelected = 0;
int enabledIA = false;

bool selected = false;
bool downed = false;

void settingsMenu() {
  u8g2.clearBuffer();
  if(indexSelected <= 2){
      if(indexSelected == 0)
        u8g2.drawStr(0, 10, "UP <-");
      else
        u8g2.drawStr(0, 10, "UP");
      u8g2.drawStr(0, 20, " Points random on eat");

      if(indexSelected == 1)
        u8g2.drawStr(0, 30, "DOWN <-");
      else
        u8g2.drawStr(0, 30, "DOWN");
      // ...

      if(indexSelected == 2)
        u8g2.drawStr(0,50,"LEFT <-");
      else
        u8g2.drawStr(0,50, "LEFT");
      // ...

      if(MAXincreace == 30)
        u8g2.drawStr(0,40, " Ball size 30");
      else
        u8g2.drawStr(0,40, " Ball size infi");
      
      if(former == 0)
        u8g2.drawStr(0,60, " Format: Circle");
      else
        u8g2.drawStr(0,60, " Format: Square");
  }
  else if(indexSelected >=  3){
    if(indexSelected == 3)
      u8g2.drawStr(0, 10, "IA <-");
    else
      u8g2.drawStr(0, 10, "IA");
    if(enabledIA)
      u8g2.drawStr(0, 20, "True");
    else
      u8g2.drawStr(0, 20, "False");
  }

  if(yValue >= 1 && downed == false){
    indexSelected++;
    downed = true;
  }
  else if(yValue <= -1 && downed == false){
    indexSelected--;
    downed = true;
  }

  if(indexSelected > 3)
    indexSelected = 0;
  else if(indexSelected < 0)
    indexSelected = 3;

  //if(BUTTON_UP.isPressed()) {
  //  digitalWrite(LED_RED, LOW);
  //  digitalWrite(LED_GREEN, HIGH);
  //  RandomOnEat = true;
  //  Settings = false;
  //  StartSettings = false;
  //  startedGame = true;
  //}
  if(xValue >= 1 && indexSelected == 0) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    RandomOnEat = true;
    Settings = false;
    StartSettings = false;
    startedGame = true;
  }
  //if(BUTTON_DOWN.isPressed()) {
  //  if(MAXincreace == 30)
  //    MAXincreace = 90000;
  //  else
  //    MAXincreace = 30;
    //digitalWrite(LED_RED, LOW);
    //digitalWrite(LED_GREEN, HIGH);
    //Settings = false;
    //StartSettings = false;
    //startedGame = true;
  //}
  if(xValue >= 1 && indexSelected == 1 && selected == false) {
    if(MAXincreace == 30)
      MAXincreace = 90000;
    else
      MAXincreace = 30;
    selected = true;
  }
  //if(BUTTON_LEFT.isPressed()) {
  //  if(former == 0){
  //    former++;
  //  }
    //else if(former == 1){
      //former++;
    //}
    //else if(former == 2){
    //  former++;
    //}
  //  else
  //    former = 0;
  //}
  if(xValue >= 1 && indexSelected == 2 && selected == false){
    if(former == 0)
      former++;
    else
      former = 0;
    selected = true;
  }
    
  //if(former == 2)
    //u8g2.drawStr(0,60, " Format: Triangle");
  
  if(xValue >= 1 && indexSelected == 3 && selected == false){
    enabledIA = !enabledIA;
    selected = true;
  }

  if(xValue == 0)
    selected = false;
  if(yValue == 0)
    downed = false;
    
  u8g2.sendBuffer();
}

void ChangePosition() {
  //if(BUTTON_UP.getState() == 1 && increace <= MAXincreace)
  //  Y++;
  //if(BUTTON_DOWN.getState() == 1 && increace <= MAXincreace)
  //  Y--;
  //if(BUTTON_LEFT.getState() == 1 && increace <= MAXincreace)
  //  X++;
  //if(BUTTON_RIGHT.getState() == 1 && increace <= MAXincreace)
  //  X--;
  if(xValue >= 1)
    X = X + xValue;
  else if(xValue < 0)
    X = X + xValue;
  if(yValue >= 1)
    Y = Y + yValue;
  else if(yValue < 0)
    Y = Y + yValue;
}
void VerifyBorder() {
  if(X >= 124)
    X -= 3;
  else if(X <= 0)
    X += 3;
  if(Y >= 64)
    Y -= 3;
  else if(Y <= 0)
    Y += 3;

  if(IAX >= 124)
    IAX--;
  else if(IAX <= 0)
    IAX++;
  
  if(IAY >= 64)
    IAY--;
  else if(IAY <= 0)
    IAY++;
}

void RandomBallPOS() {
  randPOSX = random(10,114);
  randPOSY = random(10,54);
}

void IA() {
  if(IASIZE > increace){
    if(IAX >= X)
      IAX--;
    else
      IAX++;
    
    if(IAY >= Y)
      IAY--;
    else
      IAY++;
  }else{
  if(IAX >= X)
      IAX++;
    else
      IAX--;

    if(IAY >= Y)
      IAY++;
    else
      IAY--;
  }
  u8g2.drawCircle(IAX, IAY, IASIZE);

  if(increace > IASIZE){
    if(abs(IAX - X) <= increace && abs(IAY - Y) <= increace)
      eated = true;
  }else{
    if(abs(IAX - X) <= IASIZE && abs(IAY - Y) <= IASIZE)
      GAMEOVER();
  }
  if(eated){
    tone(BUZZER, 500, 300);
    IAX = random(0,133);
    IAY = random(10,60);
    IASIZE = random(1,20);
    eated = false;
  }
}
void game() {
  VerifyBorder();
  u8g2.clearBuffer();
  if(enabledIA)
    IA();

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

  if(increace >= MAXincreace)
    WINNER();
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

void GAMEOVER() {
  u8g2.clearBuffer();
  startedGame = false;
  u8g2.drawStr(0,10, "PERDEU");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.drawStr(10,20,"PLAYBOY");
  //u8g2.drawBitmap(10, 40, sad_width, sad_height, sad_bits);
  u8g2.sendBuffer();
  delay(300);
  loosed = true;
}
void WINNER() {
  startedGame = false;
  u8g2.clearBuffer();
  u8g2.drawStr(10, 10, "parabens");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.drawStr(10,30,"Voce perdeu seu");
  //u8g2.drawBitmap(10, 40, happy_width, happy_height, happy_bits);
  u8g2.sendBuffer();
  delay(1000);
  u8g2.drawStr(62,60,"TEMPO.");
  u8g2.sendBuffer();
  delay(2000);
  loosed = true;
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
	delay(3000);
}
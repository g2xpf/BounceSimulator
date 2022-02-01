#include "M5Stack.h"

#define LGFX_USE_V1
#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>

#include <BounceSimulator.h>

// 定数
const int beepDuration = 50;
const int fontSize = 7; // M5Stack の TextSize(1) のサイズ
const int displayWidth = 320;
const int displayHeight = 240;
const int simulatorAreaWidth = 240;
const int simulatorAreaHeight = displayHeight;
const int titleAreaX = simulatorAreaWidth;
const int titleAreaY = 0;
const int titleAreaWidth = displayWidth - simulatorAreaWidth;
const int titleAreaHeight = fontSize * 5;
const int textAreaX = simulatorAreaWidth;
const int textAreaY = titleAreaHeight;
const int textAreaWidth = titleAreaWidth;
const int textAreaHeight = displayHeight - titleAreaHeight;
const int ballRadius = 30;
const double ballAccel = 30.0;

// 描画APIの初期化
static LGFX lcd;
static LGFX_Sprite simulatorSprite(&lcd);
static LGFX_Sprite textAreaSprite(&lcd);

// ボールの位置をディスプレイ座標系へ変換する関数
void ballPos2LcdCoord(double *ballPosX, double *ballPosY) {
  *ballPosX = *ballPosX + simulatorAreaWidth * 0.5;
  *ballPosY = simulatorAreaHeight - *ballPosY;
}

// タイトルの描画
void drawTitle() {
  LGFX_Sprite titleSprite(&lcd);
  titleSprite.createSprite(titleAreaWidth, titleAreaHeight);

  titleSprite.setColor(TFT_WHITE);
  titleSprite.setTextSize(1);
  titleSprite.setCursor(fontSize, fontSize);
  titleSprite.print("Emfrp");
  titleSprite.setCursor(fontSize, fontSize * 2);
  titleSprite.print("Bounce");
  titleSprite.setCursor(fontSize, fontSize * 3);
  titleSprite.print("Simulator");
  titleSprite.drawRect(0, 0, titleAreaWidth, titleAreaHeight, TFT_WHITE);
  titleSprite.pushSprite(titleAreaX, titleAreaY, TFT_BLACK);
  titleSprite.deleteSprite();
}

// ボタンのカウントを描画する関数
void drawBtnCount(int numLBtnPressed, int numCBtnPressed, int numRBtnPressed) {
  textAreaSprite.setColor(TFT_WHITE);
  textAreaSprite.setCursor(fontSize, fontSize * 1);
  textAreaSprite.printf("Left  : %d", numLBtnPressed);
  textAreaSprite.setCursor(fontSize, fontSize * 2);
  textAreaSprite.printf("Center: %d", numCBtnPressed);
  textAreaSprite.setCursor(fontSize, fontSize * 3);
  textAreaSprite.printf("Right : %d", numRBtnPressed);
  textAreaSprite.drawRect(0, 0, textAreaWidth, textAreaHeight, TFT_WHITE);
  textAreaSprite.pushSprite(textAreaX, textAreaY, TFT_RED);
}

// ボールを描画する関数
void drawBall(double *ballPosX, double *ballPosY) {
  double x = *ballPosX;
  double y = *ballPosY;
  ballPos2LcdCoord(&x, &y);

  simulatorSprite.fillRect(0, 0, simulatorAreaWidth, simulatorAreaHeight,
                           TFT_BLACK);
  simulatorSprite.setColor(TFT_WHITE);
  simulatorSprite.fillCircle((int)x, (int)y, ballRadius);
  simulatorSprite.drawRect(0, 0, simulatorAreaWidth, simulatorAreaHeight);
  simulatorSprite.pushSprite(0, 0, TFT_RED);
}

void Input(double *lbtn, int *inputLBtn, double *rbtn, int *inputRBtn,
           double *cbtn, int *inputCBtn, double *tick, int *time) {
  /* Your code goes here... */
  *inputLBtn = M5.BtnA.wasPressed();
  *inputCBtn = M5.BtnB.wasPressed();
  *inputRBtn = M5.BtnC.wasPressed();

  *lbtn = -ballAccel;
  *cbtn = ballAccel;
  *rbtn = ballAccel;

  *time = millis();
  *tick = double(*time) / 1000.0;

  M5.update();
}

void Output(double *ballPosX, double *ballPosY, int *outputDrawBall,
            int *numLBtnPressed, int *numCBtnPressed, int *numRBtnPressed,
            int *outputNumButtonPressed) {
  /* Your code goes here... */
  // ボールの描画
  if (*outputDrawBall) {
    drawBall(ballPosX, ballPosY);
  }

  // ボタンが押された時，カウントを再描画してブザーを鳴らす
  if (*outputNumButtonPressed) {
    drawBtnCount(*numLBtnPressed, *numCBtnPressed, *numRBtnPressed);

    M5.Speaker.beep();
    delay(50);
    M5.Speaker.mute();
  }
}

void setup() {
  M5.begin();
  M5.Speaker.begin();
  M5.Speaker.setVolume(1);
  M5.Speaker.setBeep(440, beepDuration);

  lcd.init();
  lcd.setRotation(1);
  lcd.setBrightness(128);

  drawTitle();

  simulatorSprite.createSprite(simulatorAreaWidth, simulatorAreaHeight);
  textAreaSprite.createSprite(textAreaWidth, textAreaHeight);
  textAreaSprite.setTextSize(1);

  drawBtnCount(0, 0, 0);
}

void loop() { ActivateBounceSimulator(); }

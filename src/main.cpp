#include "M5Stack.h"

#include <BounceSimulator.h>

#define LGFX_USE_V1
#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>

// 定数
const int fontSize = 10;
const int simulatorAreaWidth = 240;
const int simulatorAreaHeight = 240;
const int textAreaX = 240;
const int textAreaY = 0;
const int textAreaWidth = 80;
const int textAreaHeight = 240;
const int displayWidth = 320;
const int displayHeight = 240;
const int ballRadius = 30;
const float ballAccel = 30.0;

// 描画APIの初期化
static LGFX lcd;
static LGFX_Sprite simulatorSprite(&lcd);
static LGFX_Sprite textAreaSprite(&lcd);

// ボールの位置をディスプレイ座標系へ変換する関数
void ballPos2LcdCoord(float *ballPosX, float *ballPosY) {
  *ballPosX = *ballPosX + simulatorAreaWidth * 0.5;
  *ballPosY = simulatorAreaHeight - *ballPosY;
}

// ボタンのカウントを描画する関数
void drawBtnCount(int numLBtnPressed, int numCBtnPressed, int numRBtnPressed) {
  textAreaSprite.pushSprite(textAreaWidth, textAreaHeight, TFT_BLACK);
  textAreaSprite.setColor(TFT_WHITE);
  textAreaSprite.setCursor(fontSize, fontSize * 1);
  textAreaSprite.printf("Left  : %d", numLBtnPressed);
  textAreaSprite.setCursor(fontSize, fontSize * 2);
  textAreaSprite.printf("Center: %d", numCBtnPressed);
  textAreaSprite.setCursor(fontSize, fontSize * 3);
  textAreaSprite.printf("Right : %d", numRBtnPressed);
  textAreaSprite.pushSprite(textAreaX, textAreaY, TFT_RED);
}

// ボールを描画する関数
void drawBall(const float ballPosX, const float ballPosY) {
  float x = ballPosX;
  float y = ballPosY;
  ballPos2LcdCoord(&x, &y);

  simulatorSprite.fillRect(0, 0, simulatorAreaWidth, simulatorAreaHeight,
                           TFT_BLACK);
  simulatorSprite.setColor(TFT_WHITE);
  simulatorSprite.fillCircle((int)x, (int)y, ballRadius);
  simulatorSprite.drawRect(0, 0, simulatorAreaWidth, simulatorAreaHeight);
  simulatorSprite.pushSprite(0, 0, TFT_RED);
}

// 入力時変値を設定する処理を記述する．
float Input_lbtn() {
  // input lbtn
  return -ballAccel;
}
float Input_cbtn() {
  // input cbtn
  return ballAccel;
}
float Input_rbtn() {
  // input rbtn
  return ballAccel;
}
float Input_tick() {
  // input tick
  return float(millis()) * 0.001;
}

// 出力時変値を用いた処理を記述する．
void Output_ballPos(const Tuple2FloatFloat &ballPos) {
  // output ballPos

  static int callCount = 0;
  callCount++;
  // lcd.setCursor(30, 30);
  // lcd.printf("{%d} ballPos: (%lf, %lf)", callCount, ballPos._0, ballPos._1);
  drawBall(ballPos._0, ballPos._1);
}

void Output_numButtonPressed(const Tuple3IntIntInt &numButtonPressed) {
  // output numButtonPressed

  static int callCount = 0;
  callCount++;
  lcd.setCursor(30, 30 + fontSize);
  // lcd.printf("{%d} press state: (%d, %d, %d)", callCount,
  // numButtonPressed._0,
  //            numButtonPressed._1, numButtonPressed._2);

  drawBtnCount(numButtonPressed._0, numButtonPressed._1,
               numButtonPressed._2); // M5.Speaker.tone(440, 50);
}

hw_timer_t *timer1 = nullptr;
void InitTimer(int offset_ms, int interval_ms) {
  // ここでタイマーの初期化を行う．
  // 引数の offset と interval は PbEmfrp
  // の処理系によって計算されたタイマー割り
  // 込みの開始時刻と周期であり，この値を用いてプログラマが
  // タイマーを初期化する．

  M5.begin();
  M5.Speaker.setVolume(1);

  lcd.init();
  lcd.setRotation(1);
  lcd.setBrightness(128);

  simulatorSprite.createSprite(simulatorAreaWidth, simulatorAreaHeight);
  textAreaSprite.createSprite(textAreaWidth, textAreaHeight);

  drawBtnCount(0, 0, 0);

  // 各種割り込み設定
  timer1 = timerBegin(0, 80, true);

  timerAttachInterrupt(timer1, &timerISR, true);
  timerAlarmWrite(timer1, interval_ms * 1000, true);
  timerAlarmEnable(timer1);

  // 左ボタン
  pinMode(GPIO_NUM_39, INPUT);
  attachInterrupt(GPIO_NUM_39, lbtnISR, FALLING);

  // 中央ボタン
  pinMode(GPIO_NUM_38, INPUT);
  attachInterrupt(GPIO_NUM_38, cbtnISR, FALLING);

  // 右ボタン
  pinMode(GPIO_NUM_37, INPUT);
  attachInterrupt(GPIO_NUM_37, rbtnISR, FALLING);
}

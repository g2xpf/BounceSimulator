#include "BounceSimulator.h"
// #include <freertos/FreeRTOS.h>
#include <M5Stack.h>

// 時変値を管理するグローバル変数の宣言
// 入力時変値
float node_lbtn[1];
int node_lbtn_index = 0;
float node_cbtn[1];
int node_cbtn_index = 0;
float node_rbtn[1];
int node_rbtn_index = 0;
float node_tick[1];
int node_tick_index = 0;

// 出力時変値
Tuple2FloatFloat node_ballPos[1];
int node_ballPos_index = 0;
Tuple3IntIntInt node_numButtonPressed[1];
int node_numButtonPressed_index = 0;

// その他の時変値
const float node_DELTA_ACCEL = 2.0;
const float node_BALL_RADIUS = 30.0;
const float node_GRAVITY_X = 0.0;
const float node_GRAVITY_Y = -9.8;
const float node_LEFT_WALL_POS = -120.0;
const float node_RIGHT_WALL_POS = 120.0;
const float node_FLOOR_POS = 0.0;
const float node_E = 0.7;
const float node_INIT_BALL_HEIGHT = node_BALL_RADIUS + 50.0;
const float node_accelX = node_GRAVITY_X;
const float node_accelY = node_GRAVITY_Y;

int node_numLBtnPressed[2];
int node_numLBtnPressed_index = 0;
int node_numCBtnPressed[2];
int node_numCBtnPressed_index = 0;
int node_numRBtnPressed[2];
int node_numRBtnPressed_index = 0;
Tuple3IntIntInt node_numLBtnPressedSync[1];
int node_numLBtnPressedSync_index = 0;
Tuple3IntIntInt node_numCBtnPressedSync[1];
int node_numCBtnPressedSync_index = 0;
Tuple3IntIntInt node_numRBtnPressedSync[1];
int node_numRBtnPressedSync_index = 0;
float node_tickHistory[2];
int node_tickHistory_index = 0;
float node_dt[1];
int node_dt_index = 0;
float node_velocityX[1];
int node_velocityX_index = 0;
float node_velocityY[1];
int node_velocityY_index = 0;
float node_lastPositionX[1];
int node_lastPositionX_index = 0;
float node_lastPositionY[1];
int node_lastPositionY_index = 0;
float node_reflectedVelocityX[2];
int node_reflectedVelocityX_index = 0;
float node_reflectedVelocityY[2];
int node_reflectedVelocityY_index = 0;
float node_positionX[2];
int node_positionX_index = 0;
float node_positionY[2];
int node_positionY_index = 0;

inline int node_numCBtnPressed_now() {
  return node_numCBtnPressed[node_numCBtnPressed_index];
}
inline int node_numRBtnPressed_now() {
  return node_numRBtnPressed[node_numRBtnPressed_index];
}
inline int node_numLBtnPressed_now() {
  return node_numLBtnPressed[node_numLBtnPressed_index];
}
inline float node_lastPositionX_now() {
  return node_lastPositionX[node_lastPositionX_index];
}
inline float node_lastPositionY_now() {
  return node_lastPositionY[node_lastPositionY_index];
}

// 関数
float toCount(float accel) { return 1; }
Tuple3IntIntInt mergeTupleInt3(Tuple3IntIntInt lhs, Tuple3IntIntInt rhs) {
  return lhs;
}
float mergeFloat(float lhs, float rhs) { return 1; }

// ThermoHygrometerMain.cpp で定義される関数の宣言
// 入力時変値
extern float Input_lbtn();
extern float Input_rbtn();
extern float Input_cbtn();
extern float Input_tick();
// 出力時変値
extern void Output_ballPos(const Tuple2FloatFloat &ballPos);
extern void Output_numButtonPressed(const Tuple3IntIntInt &numButtonPressed);

extern void InitTimer(int offset_ms, int interval_ms);

// ここで全時変値の初期化を行う
void InitBounceSimulator() {
  node_numLBtnPressed[0] = 0;
  node_numCBtnPressed[0] = 0;
  node_numRBtnPressed[0] = 0;
  node_tickHistory[0] = -1.0;
  node_reflectedVelocityX[0] = 0.0;
  node_reflectedVelocityY[0] = 0.0;
  node_positionX[0] = 0.0;
  node_positionY[0] = node_INIT_BALL_HEIGHT;
}

// ----------------------------------------------------------------

// 入力時変値から出力時変値の計算を行う．
#define TIMER_ISR_BIT 0x01
#define LBTN_ISR_BIT 0x02
#define RBTN_ISR_BIT 0x03
#define CBTN_ISR_BIT 0x04

// タスクハンドラ
static TaskHandle_t xHandlingTask;

// タイマー割り込みのハンドラの生成
void timerISR(void) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xTaskNotifyFromISR(xHandlingTask, TIMER_ISR_BIT, eSetBits,
                     &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

// 非周期的時変値の割り込みハンドラ
void lbtnISR(void) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xTaskNotifyFromISR(xHandlingTask, LBTN_ISR_BIT, eSetBits,
                     &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

void rbtnISR(void) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xTaskNotifyFromISR(xHandlingTask, RBTN_ISR_BIT, eSetBits,
                     &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

void cbtnISR(void) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xTaskNotifyFromISR(xHandlingTask, CBTN_ISR_BIT, eSetBits,
                     &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

// 時変値更新を行うかを決定するフラグ
// '(1000, 100)
bool cond_1;

// タイマー割り込みの回数を数えるカウンタ
int Counter = 0;
void Update_timer() {
  // タイマー割り込み回数を更新
  Counter++;

  // フラグの更新
  cond_1 = Counter >= 10 && (Counter - 10) % 1 == 0;

  // インデックスの更新
  if (cond_1) {
    node_tick_index = (node_tick_index + 0) % 1;
    node_tickHistory_index = (node_tickHistory_index + 1) % 2;
    node_dt_index = (node_dt_index + 0) % 1;
    node_velocityX_index = (node_velocityX_index + 0) % 1;
    node_velocityY_index = (node_velocityY_index + 0) % 1;
    node_lastPositionX_index = (node_lastPositionX_index + 0) % 1;
    node_lastPositionY_index = (node_lastPositionY_index + 0) % 1;
    node_reflectedVelocityX_index = (node_reflectedVelocityX_index + 1) % 2;
    node_reflectedVelocityY_index = (node_reflectedVelocityY_index + 1) % 2;
    node_positionX_index = (node_positionX_index + 1) % 2;
    node_positionY_index = (node_positionY_index + 1) % 2;
    node_ballPos_index = (node_ballPos_index + 0) % 1;
  }

  // 入力時変値の設定
  if (cond_1) {
    node_tick[node_tick_index] = Input_tick();
  }

  // 時変値の更新
  if (cond_1) {
    node_tickHistory[node_tickHistory_index] = node_tick[node_tick_index];
    node_dt[node_dt_index] =
        node_tickHistory[(node_tickHistory_index + 1) % 2] < 0.0
            ? 0.0
            : node_tickHistory[node_tickHistory_index] -
                  node_tickHistory[(node_tickHistory_index + 1) % 2];
    node_velocityX[node_velocityX_index] =
        node_reflectedVelocityX[(node_reflectedVelocityX_index + 1) % 2] +
        node_accelX * node_dt[node_dt_index];
    node_velocityY[node_velocityY_index] =
        node_reflectedVelocityY[(node_reflectedVelocityY_index + 1) % 2] +
        node_accelY * node_dt[node_dt_index];
    node_lastPositionX[node_lastPositionX_index] =
        node_positionX[(node_positionX_index + 1) % 2];
    node_lastPositionY[node_lastPositionY_index] =
        node_positionY[(node_positionY_index + 1) % 2];
    node_reflectedVelocityX[node_reflectedVelocityX_index] =
        node_velocityX[node_velocityX_index] *
        (((node_lastPositionX_now() - node_BALL_RADIUS < node_LEFT_WALL_POS &&
           node_velocityX[node_velocityX_index] < 0.0) ||
          (node_lastPositionX_now() + node_BALL_RADIUS > node_RIGHT_WALL_POS &&
           node_velocityX[node_velocityX_index] > 0.0))
             ? -node_E
             : 1.0);
    node_reflectedVelocityY[node_reflectedVelocityY_index] =
        node_velocityY[node_velocityY_index] *
        ((node_lastPositionY_now() - node_BALL_RADIUS < node_FLOOR_POS &&
          node_velocityY[node_velocityY_index] < -0.0)
             ? -node_E
             : 1.0);
    node_positionX[node_positionX_index] =
        node_lastPositionX[node_lastPositionX_index] +
        node_reflectedVelocityX[node_reflectedVelocityX_index] *
            node_dt[node_dt_index];
    node_positionY[node_positionY_index] =
        node_lastPositionY[node_lastPositionY_index] +
        node_reflectedVelocityY[node_reflectedVelocityY_index] *
            node_dt[node_dt_index];
    node_ballPos[node_ballPos_index] = Tuple2FloatFloat{
        ._0 = node_positionX[node_positionX_index],
        ._1 = node_positionY[node_positionY_index],
    };
  }

  // 出力
  if (cond_1) {
    Output_ballPos(node_ballPos[node_ballPos_index]);
  }
}

void Update_lbtn() {
  // インデックスの更新
  node_lbtn_index = (node_lbtn_index + 0) % 1;
  node_numLBtnPressed_index = (node_numLBtnPressed_index + 1) % 2;
  node_numLBtnPressedSync_index = (node_numLBtnPressedSync_index + 0) % 1;
  node_velocityX_index = (node_velocityX_index + 0) % 1;
  node_reflectedVelocityX_index = (node_reflectedVelocityX_index + 0) % 1;
  node_numButtonPressed_index = (node_numButtonPressed_index + 0) % 1;

  // 入力時変値の設定
  node_lbtn[node_lbtn_index] = Input_lbtn();

  // 時変値の更新
  node_numLBtnPressed[node_numLBtnPressed_index] =
      node_numLBtnPressed[(node_numLBtnPressed_index + 1) % 2] +
      toCount(node_lbtn[node_lbtn_index]);
  node_numLBtnPressedSync[node_numLBtnPressedSync_index] = Tuple3IntIntInt{
      ._0 = node_numLBtnPressed[node_numLBtnPressed_index],
      ._1 = node_numCBtnPressed_now(),
      ._2 = node_numRBtnPressed_now(),
  };
  node_numButtonPressed[node_numButtonPressed_index] =
      node_numLBtnPressedSync[node_numLBtnPressedSync_index];
  node_velocityX[node_velocityX_index] =
      node_reflectedVelocityX[(node_reflectedVelocityX_index + 1) % 2] +
      node_lbtn[node_lbtn_index];
  node_reflectedVelocityX[node_reflectedVelocityX_index] =
      node_velocityX[node_velocityX_index] *
      ((node_lastPositionY_now() - node_BALL_RADIUS < node_FLOOR_POS &&
        node_velocityY[node_velocityY_index] < 0.0)
           ? -node_E
           : 1.0);

  //出力
  Output_numButtonPressed(node_numButtonPressed[node_numButtonPressed_index]);
}

void Update_cbtn() {
  // インデックスの更新
  node_cbtn_index = (node_cbtn_index + 0) % 1;
  node_numCBtnPressed_index = (node_numCBtnPressed_index + 1) % 2;
  node_numCBtnPressedSync_index = (node_numCBtnPressedSync_index + 0) % 1;
  node_velocityY_index = (node_velocityY_index + 0) % 1;
  node_reflectedVelocityY_index = (node_reflectedVelocityY_index + 1) % 2;
  node_numButtonPressed_index = (node_numButtonPressed_index + 0) % 1;

  // 入力時変値の設定
  node_cbtn[node_cbtn_index] = Input_cbtn();

  // 時変値の更新
  node_numCBtnPressed[node_numCBtnPressed_index] =
      node_numCBtnPressed[(node_numCBtnPressed_index + 1) % 2] +
      toCount(node_cbtn[node_cbtn_index]);
  node_numCBtnPressedSync[node_numCBtnPressedSync_index] = Tuple3IntIntInt{
      ._0 = node_numLBtnPressed_now(),
      ._1 = node_numCBtnPressed[node_numCBtnPressed_index],
      ._2 = node_numRBtnPressed_now(),
  };
  node_numButtonPressed[node_numButtonPressed_index] =
      node_numCBtnPressedSync[node_numCBtnPressedSync_index];
  node_velocityY[node_velocityY_index] =
      node_reflectedVelocityY[(node_reflectedVelocityY_index + 1) % 2] +
      node_cbtn[node_cbtn_index];
  node_reflectedVelocityY[node_reflectedVelocityY_index] =
      node_velocityY[node_velocityY_index] *
      ((node_lastPositionY_now() - node_BALL_RADIUS < node_FLOOR_POS &&
        node_velocityY[node_velocityY_index] < -0.0)
           ? -node_E
           : 1.0);

  //出力
  Output_numButtonPressed(node_numButtonPressed[node_numButtonPressed_index]);
}

void Update_rbtn() {
  // インデックスの更新
  node_rbtn_index = (node_rbtn_index + 0) % 1;
  node_numRBtnPressed_index = (node_numRBtnPressed_index + 1) % 2;
  node_numRBtnPressedSync_index = (node_numRBtnPressedSync_index + 0) % 1;
  node_velocityX_index = (node_velocityX_index + 0) % 1;
  node_reflectedVelocityX_index = (node_reflectedVelocityX_index + 1) % 2;
  node_numButtonPressed_index = (node_numButtonPressed_index + 0) % 1;

  // 入力時変値の設定
  node_rbtn[node_rbtn_index] = Input_rbtn();

  // 時変値の更新
  node_numRBtnPressed[node_numRBtnPressed_index] =
      node_numRBtnPressed[(node_numRBtnPressed_index + 1) % 2] +
      toCount(node_rbtn[node_rbtn_index]);
  node_numRBtnPressedSync[node_numRBtnPressedSync_index] = Tuple3IntIntInt{
      ._0 = node_numLBtnPressed_now(),
      ._1 = node_numCBtnPressed_now(),
      ._2 = node_numRBtnPressed[node_numRBtnPressed_index],
  };
  node_numButtonPressed[node_numButtonPressed_index] =
      node_numRBtnPressedSync[node_numRBtnPressedSync_index];
  node_velocityX[node_velocityX_index] =
      node_reflectedVelocityX[(node_reflectedVelocityX_index + 1) % 2] +
      node_rbtn[node_rbtn_index];
  node_reflectedVelocityX[node_reflectedVelocityX_index] =
      node_velocityX[node_velocityX_index] *
      ((node_lastPositionX_now() - node_BALL_RADIUS < node_FLOOR_POS &&
        node_velocityX[node_velocityX_index] < 0.0)
           ? -node_E
           : 1.0);

  //出力
  Output_numButtonPressed(node_numButtonPressed[node_numButtonPressed_index]);
}

void mainTask(void *pvParameter) {
  uint32_t ulNotifiedValue;
  BaseType_t xResult;
  for (;;) {
    xResult =
        xTaskNotifyWait(pdFALSE, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);
    if (xResult == pdPASS) {
      switch (ulNotifiedValue) {
      case TIMER_ISR_BIT:
        Update_timer();
        break;
      case LBTN_ISR_BIT:
        Update_lbtn();
        break;
      case CBTN_ISR_BIT:
        Update_cbtn();
        break;
      case RBTN_ISR_BIT:
        Update_rbtn();
        break;
      }
    }
  }
}

// メイン関数
void setup() {
  InitTimer(1000, 100);

  InitBounceSimulator();

  // タスクの生成
  static uint8_t ucParameterToPass;
  xTaskCreate(mainTask, "BounceSimulatorRuntime", 4096, &ucParameterToPass, 1,
              &xHandlingTask);
  configASSERT(xHandlingTask);
}

void loop() { delay(1); }

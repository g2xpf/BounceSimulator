#include "BounceSimulator.h"

bool cond_1;
bool cond_2;
bool cond_3;
bool cond_4;

// 時変値処理を行う割り込みハンドラ
void ISR() {
  cond_1 = Counter >= 5 && (Counter - 5) % 5 == 0;
  cond_2 = Counter >= 5 && (Counter - 5) % 3 == 0;
  cond_3 = Counter >= 20 && (Counter - 20) % 30 == 0;
  cond_4 = Counter >= 18 && (Counter - 18) % 30 == 0;
  node_tmp_index = (node_tmp_index - cond_1 + 1) % 1;
  node_tmpHistory_index = (node_tmpHistory_index - cond_1 + 4) % 4;
  node_hmd_index = (node_tmpHistory_index - cond_2 + 1) % 1;
  node_tmpAve_index = (node_tmpAve_index - cond_1 + 1) % 1;
  node_sendTmpAve_index = (node_sendTmpAve_index - cond_3 + 1) % 1;
  node_hmdHistory_index = (node_hmdHistory_index - cond_2 + 3) % 3;
  node_hmdAve_index = (node_hmdAve_index - cond_2 + 1) % 1;
  node_sendHmdAve_index = (node_sendHmdAve_index - cond_4 + 1) % 1;
  node_display_index = (node_display_index - (cond_1 || cond_2) + 1) % 1;

  // 入力時変値の処理
  if (cond_1) {
    node_tmp[node_tmp_index] = Input_tmp();
  }
  if (cond_2) {
    node_tmp[node_tmp_index] = Input_hmd();
  }
  // 時変値計算
  MainTask();
  // 出力時変値の処理
  if (cond_3) {
    Output_sendTmpAve(node_sendTmpAve[node_sendTmpAve_index]);
  }
  if (cond_4) {
    Output_sendHmdAve(node_sendHmdAve[node_sendHmdAve_index]);
  }
  if (cond_1 || cond_2) {
    Output_display(node_display[node_display_index]);
  }
  Counter++;
  // 割り込み許可フラグのリセット．
  ResetInterruptFlag();
}

// 入力時変値から出力時変値の計算を行う．
void MainTask() {
  if (cond_1) {
    node_tmpAve[node_tmpAve_index] =
        (node_tmpHistory[node_tmpAve_index] +
         node_tmpHistory[(node_tmpAve_index + 1) % 4] +
         node_tmpHistory[(node_tmpAve_index + 2) % 4] +
         node_tmpHistory[(node_tmpAve_index + 3) % 4]) /
        4.0;
    if (cond_3) {
      node_sendTmpAve[node_sendTmpAve_index] = node_tmpAve[node_tmpAve_index];
    }
  }
  if (cond_2) {
    node_hmdAve[node_hmdAve_index] =
        (node_hmdHistory[node_hmdAve_index] +
         node_hmdHistory[(node_hmdAve_index + 1) % 3] +
         node_hmdHistory[(node_hmdAve_index + 2) % 3]) /
        3.0;
    if (cond_4) {
      node_sendHmdAve[node_sendHmdAve_index] = node_hmdAve[node_hmdAve_index];
    }
  }
  if (cond_1 || cond_2) {
    Tuple2DoubleDouble _tmp1 = {node_tmp[node_tmp_index],
                                node_hmdHistory_now()};
    Tuple2DoubleDouble _tmp2 = {node_tmpHistory_now(),
                                node_hmd[node_hmd_index]};
    Tuple2DoubleDouble _tmp3;
    if (cond_1 && cond_2) {
      _tmp3 = makePair(_tmp1, _tmp2);
    } else if (cond_1) {
      _tmp3 = _tmp1;
    } else if (cond_2) {
      _tmp3 = _tmp2;
    }
    node_display[node_display_index] = calcDisplay(_tmp3);
  }
}

#define TIMER_ISR_BIT 0x01
#define LBTN_ISR_BIT 0x02
#define RBTN_ISR_BIT 0x03
#define CBTN_ISR_BIT 0x04

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

// メイン関数
void setup() {
  InitBounceSimulator();

  // タスクの生成
  static uint8_t ucParameterToPass;
  xTaskCreate(mainHandler, "BounceSimulatorRuntime", 4096, &ucParameterToPass,
              1, &xHandlingTask);
  configASSERT(xHandlingTask);
}

void loop() { delay(1); }

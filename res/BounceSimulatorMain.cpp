#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "BounceSimulator.h"

// 入力時変値を設定する処理を記述する．
float Input_lbtn() {
  // input lbtn
}
float Input_cbtn() {
  // input cbtn
}
float Input_rbtn() {
  // input rbtn
}
float Input_tick() {
  // input tick
}

// 出力時変値を用いた処理を記述する．
void Output_ballPos(const Tuple2FloatFloat &ballPos) {
  // output ballPos
}

void Output_numButtonPressed(const Tuple3IntIntInt &numButtonPressed) {
  // output numButtonPressed
}

void InitTimer(int offset_ms, int interval_ms) {
  // ここでタイマーの初期化を行う．
  //
  // 引数の offset と interval は PbEmfrp
  // の処理系によって計算されたタイマー割り
  // 込みの開始時刻と周期であり，この値を用いてプログラマが
  // タイマーを初期化する．
}

void ResetInterruptFlag() {
  // 設定された割り込みフラグをリセットする処理を記述する．
  // この関数は割り込みハンドラの最後に呼ばれる．
}

void InitBounceSimulator() {
  // ここでハードウェアの初期化を行う
}

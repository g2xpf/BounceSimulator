#include "BounceSimulator.h"
/* Primitive functions (Macros) */
#define _plus__dot_(a, b) (a + b)
#define _asterisk__dot_(a, b) (a * b)
#define _gt_(a, b) (a > b)
#define _plus_(a, b) (a + b)
#define _vertial__vertial_(a, b) (a || b)
#define _anpersand_anpersand(a, b) (a && b)
#define _lt__dot_(a, b) (a < b)
#define _minus__dot_(a, b) (a - b)
#define _gt__dot_(a, b) (a > b)
#define _at__minus__dot_(a) (-a)
/* Data types */
/* Global variables */
double node_memory_lbtn[2];
int node_memory_inputLBtn[2];
double node_memory_rbtn[2];
int node_memory_inputRBtn[2];
double node_memory_cbtn[2];
int node_memory_inputCBtn[2];
double node_memory_tick[2];
int node_memory_time[2];
int node_memory_inputTick[2];
double node_memory_accelX[2];
double node_memory_accelY[2];
int node_memory_outputNumButtonPressed[2];
int node_memory_inputTickTiming[2];
double node_memory_tickHistory[2];
int node_memory_outputDrawBall[2];
int node_memory_numLBtnPressed[2];
int node_memory_numCBtnPressed[2];
int node_memory_numRBtnPressed[2];
double node_memory_dt[2];
double node_memory_velocityX[2];
double node_memory_velocityY[2];
double node_memory_reflectedVelocityX[2];
double node_memory_reflectedVelocityY[2];
double node_memory_positionX[2];
double node_memory_positionY[2];
double node_memory_ballPosX[2];
double node_memory_ballPosY[2];
double gravityY;
double floorPos;
double gravityX;
double e;
double rightWallPos;
double leftWallPos;
double ballRadius;
int Counter = 1;
int NodeSize = 19;
/* Static prototypes */
static int node_inputTick(int, int, int*);
static int node_accelX(double*);
static int node_accelY(double*);
static int node_outputNumButtonPressed(int, int, int, int*);
static int node_inputTickTiming(int, int, int*);
static int init_inputTickTiming();
static int node_tickHistory(int, double, double, double*);
static double init_tickHistory();
static int node_outputDrawBall(int, int*);
static int node_numLBtnPressed(int, int, int, int*);
static int init_numLBtnPressed();
static int node_numCBtnPressed(int, int, int, int*);
static int init_numCBtnPressed();
static int node_numRBtnPressed(int, int, int, int*);
static int init_numRBtnPressed();
static int node_dt(double, double, double*);
static int node_velocityX(double, int, double, double, int, double, int, double, double*);
static int node_velocityY(double, int, double, double, int, double, double*);
static int node_reflectedVelocityX(double, int, double, double*);
static double init_reflectedVelocityX();
static int node_reflectedVelocityY(double, int, double, double*);
static double init_reflectedVelocityY();
static int node_positionX(double, int, double, double, double*);
static double init_positionX();
static int node_positionY(double, int, double, double, double*);
static double init_positionY();
static int node_ballPosX(double, double*);
static int node_ballPosY(double, double*);
static double init_gravityY();
static double init_floorPos();
static double init_gravityX();
static double init_e();
static double init_rightWallPos();
static double init_leftWallPos();
static double init_ballRadius();
static void refreshMark();
extern void Input(double*, int*, double*, int*, double*, int*, double*, int*);
extern void Output(double*, double*, int*, int*, int*, int*, int*);
/* Functions, Constructors, GCMarkers, etc... */
static int node_inputTick(int time, int inputTickTiming_at_last, int* output) {
  *output = _gt_(time, inputTickTiming_at_last);
  return 1;
}
static int node_accelX(double* output) {
  *output = gravityX;
  return 1;
}
static int node_accelY(double* output) {
  *output = gravityY;
  return 1;
}
static int node_outputNumButtonPressed(int inputLBtn, int inputRBtn, int inputCBtn, int* output) {
  *output = _vertial__vertial_(inputLBtn, _vertial__vertial_(inputRBtn, inputCBtn));
  return 1;
}
static int node_inputTickTiming(int inputTickTiming_at_last, int inputTick, int* output) {
  int _tmp000;
  if (inputTick == 1) {
    _tmp000 = 100;
  }
  else {
    _tmp000 = 0;
  }
  *output = _plus_(inputTickTiming_at_last, _tmp000);
  return 1;
}
static int init_inputTickTiming() {
  return 1000;
}
static int node_tickHistory(int inputTick, double tick, double tickHistory_at_last, double* output) {
  double _tmp001;
  if (inputTick == 1) {
    _tmp001 = tick;
  }
  else {
    _tmp001 = tickHistory_at_last;
  }
  *output = _tmp001;
  return 1;
}
static double init_tickHistory() {
  return _at__minus__dot_(1.0);
}
static int node_outputDrawBall(int inputTick, int* output) {
  *output = inputTick;
  return 1;
}
static int node_numLBtnPressed(int outputNumButtonPressed, int inputLBtn, int numLBtnPressed_at_last, int* output) {
  int _tmp002;
  if (outputNumButtonPressed == 1) {
    int _tmp003;
    if (1) {
      int pvar0_cnt = numLBtnPressed_at_last;
      int _tmp004;
      if (inputLBtn == 1) {
        _tmp004 = 1;
      }
      else {
        _tmp004 = 0;
      }
      _tmp003 = _plus_(pvar0_cnt, _tmp004);
    }
    _tmp002 = _tmp003;
  }
  else {
    _tmp002 = numLBtnPressed_at_last;
  }
  *output = _tmp002;
  return 1;
}
static int init_numLBtnPressed() {
  return 0;
}
static int node_numCBtnPressed(int outputNumButtonPressed, int inputCBtn, int numCBtnPressed_at_last, int* output) {
  int _tmp005;
  if (outputNumButtonPressed == 1) {
    int _tmp006;
    if (1) {
      int pvar1_cnt = numCBtnPressed_at_last;
      int _tmp007;
      if (inputCBtn == 1) {
        _tmp007 = 1;
      }
      else {
        _tmp007 = 0;
      }
      _tmp006 = _plus_(pvar1_cnt, _tmp007);
    }
    _tmp005 = _tmp006;
  }
  else {
    _tmp005 = numCBtnPressed_at_last;
  }
  *output = _tmp005;
  return 1;
}
static int init_numCBtnPressed() {
  return 0;
}
static int node_numRBtnPressed(int outputNumButtonPressed, int inputRBtn, int numRBtnPressed_at_last, int* output) {
  int _tmp008;
  if (outputNumButtonPressed == 1) {
    int _tmp009;
    if (1) {
      int pvar2_cnt = numRBtnPressed_at_last;
      int _tmp010;
      if (inputRBtn == 1) {
        _tmp010 = 1;
      }
      else {
        _tmp010 = 0;
      }
      _tmp009 = _plus_(pvar2_cnt, _tmp010);
    }
    _tmp008 = _tmp009;
  }
  else {
    _tmp008 = numRBtnPressed_at_last;
  }
  *output = _tmp008;
  return 1;
}
static int init_numRBtnPressed() {
  return 0;
}
static int node_dt(double tickHistory_at_last, double tickHistory, double* output) {
  int _tmp012;
  double _tmp011;
  _tmp012 = _lt__dot_(tickHistory_at_last, 0.0);
  if (_tmp012 == 1) {
    _tmp011 = 0.0;
  }
  else {
    _tmp011 = _minus__dot_(tickHistory, tickHistory_at_last);
  }
  *output = _tmp011;
  return 1;
}
static int node_velocityX(double reflectedVelocityX_at_last, int inputTick, double accelX, double dt, int inputLBtn, double lbtn, int inputRBtn, double rbtn, double* output) {
  double _tmp013;
  if (inputTick == 1) {
    _tmp013 = _asterisk__dot_(accelX, dt);
  }
  else {
    _tmp013 = 0.0;
  }
  double _tmp014;
  if (inputLBtn == 1) {
    _tmp014 = lbtn;
  }
  else {
    _tmp014 = 0.0;
  }
  double _tmp015;
  if (inputRBtn == 1) {
    _tmp015 = rbtn;
  }
  else {
    _tmp015 = 0.0;
  }
  *output = _plus__dot_(_plus__dot_(_plus__dot_(reflectedVelocityX_at_last, _tmp013), _tmp014), _tmp015);
  return 1;
}
static int node_velocityY(double reflectedVelocityY_at_last, int inputTick, double accelY, double dt, int inputCBtn, double cbtn, double* output) {
  double _tmp016;
  if (inputTick == 1) {
    _tmp016 = _asterisk__dot_(accelY, dt);
  }
  else {
    _tmp016 = 0.0;
  }
  double _tmp017;
  if (inputCBtn == 1) {
    _tmp017 = cbtn;
  }
  else {
    _tmp017 = 0.0;
  }
  *output = _plus__dot_(_plus__dot_(reflectedVelocityY_at_last, _tmp016), _tmp017);
  return 1;
}
static int node_reflectedVelocityX(double velocityX, int inputTick, double positionX_at_last, double* output) {
  double _tmp018;
  if (inputTick == 1) {
    int _tmp020;
    double _tmp019;
    _tmp020 = _vertial__vertial_(_anpersand_anpersand(_lt__dot_(_minus__dot_(positionX_at_last, ballRadius), leftWallPos), _lt__dot_(velocityX, 0.0)), _anpersand_anpersand(_gt__dot_(_plus__dot_(positionX_at_last, ballRadius), rightWallPos), _gt__dot_(velocityX, 0.0)));
    if (_tmp020 == 1) {
      _tmp019 = _at__minus__dot_(e);
    }
    else {
      _tmp019 = 1.0;
    }
    _tmp018 = _tmp019;
  }
  else {
    _tmp018 = 1.0;
  }
  *output = _asterisk__dot_(velocityX, _tmp018);
  return 1;
}
static double init_reflectedVelocityX() {
  return 0.0;
}
static int node_reflectedVelocityY(double velocityY, int inputTick, double positionY_at_last, double* output) {
  double _tmp021;
  if (inputTick == 1) {
    int _tmp023;
    double _tmp022;
    _tmp023 = _anpersand_anpersand(_lt__dot_(_minus__dot_(positionY_at_last, ballRadius), floorPos), _lt__dot_(velocityY, 0.0));
    if (_tmp023 == 1) {
      _tmp022 = _at__minus__dot_(e);
    }
    else {
      _tmp022 = 1.0;
    }
    _tmp021 = _tmp022;
  }
  else {
    _tmp021 = 1.0;
  }
  *output = _asterisk__dot_(velocityY, _tmp021);
  return 1;
}
static double init_reflectedVelocityY() {
  return 0.0;
}
static int node_positionX(double positionX_at_last, int inputTick, double reflectedVelocityX, double dt, double* output) {
  double _tmp024;
  if (inputTick == 1) {
    _tmp024 = _asterisk__dot_(reflectedVelocityX, dt);
  }
  else {
    _tmp024 = 0.0;
  }
  *output = _plus__dot_(positionX_at_last, _tmp024);
  return 1;
}
static double init_positionX() {
  return 0.0;
}
static int node_positionY(double positionY_at_last, int inputTick, double reflectedVelocityY, double dt, double* output) {
  double _tmp025;
  if (inputTick == 1) {
    _tmp025 = _asterisk__dot_(reflectedVelocityY, dt);
  }
  else {
    _tmp025 = 0.0;
  }
  *output = _plus__dot_(positionY_at_last, _tmp025);
  return 1;
}
static double init_positionY() {
  return 80.0;
}
static int node_ballPosX(double positionX, double* output) {
  *output = positionX;
  return 1;
}
static int node_ballPosY(double positionY, double* output) {
  *output = positionY;
  return 1;
}
static double init_gravityY() {
  return _at__minus__dot_(9.8);
}
static double init_floorPos() {
  return 0.0;
}
static double init_gravityX() {
  return 0.0;
}
static double init_e() {
  return 0.7;
}
static double init_rightWallPos() {
  return 120.0;
}
static double init_leftWallPos() {
  return _at__minus__dot_(120.0);
}
static double init_ballRadius() {
  return 30.0;
}
static void refreshMark() {
  int i;
}
void ActivateBounceSimulator() {
  int current_side = 0, last_side = 1;
  node_memory_inputTickTiming[last_side] = init_inputTickTiming();
  node_memory_tickHistory[last_side] = init_tickHistory();
  node_memory_numLBtnPressed[last_side] = init_numLBtnPressed();
  node_memory_numCBtnPressed[last_side] = init_numCBtnPressed();
  node_memory_numRBtnPressed[last_side] = init_numRBtnPressed();
  node_memory_reflectedVelocityX[last_side] = init_reflectedVelocityX();
  node_memory_reflectedVelocityY[last_side] = init_reflectedVelocityY();
  node_memory_positionX[last_side] = init_positionX();
  node_memory_positionY[last_side] = init_positionY();
  gravityY = init_gravityY();
  floorPos = init_floorPos();
  gravityX = init_gravityX();
  e = init_e();
  rightWallPos = init_rightWallPos();
  leftWallPos = init_leftWallPos();
  ballRadius = init_ballRadius();
  Counter = NodeSize + 1;
  refreshMark();
  while (1) {
    Counter = 1;
    Input(&node_memory_lbtn[current_side], &node_memory_inputLBtn[current_side], &node_memory_rbtn[current_side], &node_memory_inputRBtn[current_side], &node_memory_cbtn[current_side], &node_memory_inputCBtn[current_side], &node_memory_tick[current_side], &node_memory_time[current_side]);
    node_inputTick(node_memory_time[current_side], node_memory_inputTickTiming[last_side], &node_memory_inputTick[current_side]);
    Counter++;
    node_accelX(&node_memory_accelX[current_side]);
    Counter++;
    node_accelY(&node_memory_accelY[current_side]);
    Counter++;
    node_outputNumButtonPressed(node_memory_inputLBtn[current_side], node_memory_inputRBtn[current_side], node_memory_inputCBtn[current_side], &node_memory_outputNumButtonPressed[current_side]);
    Counter++;
    node_inputTickTiming(node_memory_inputTickTiming[last_side], node_memory_inputTick[current_side], &node_memory_inputTickTiming[current_side]);
    Counter++;
    node_tickHistory(node_memory_inputTick[current_side], node_memory_tick[current_side], node_memory_tickHistory[last_side], &node_memory_tickHistory[current_side]);
    Counter++;
    node_outputDrawBall(node_memory_inputTick[current_side], &node_memory_outputDrawBall[current_side]);
    Counter++;
    node_numLBtnPressed(node_memory_outputNumButtonPressed[current_side], node_memory_inputLBtn[current_side], node_memory_numLBtnPressed[last_side], &node_memory_numLBtnPressed[current_side]);
    Counter++;
    node_numCBtnPressed(node_memory_outputNumButtonPressed[current_side], node_memory_inputCBtn[current_side], node_memory_numCBtnPressed[last_side], &node_memory_numCBtnPressed[current_side]);
    Counter++;
    node_numRBtnPressed(node_memory_outputNumButtonPressed[current_side], node_memory_inputRBtn[current_side], node_memory_numRBtnPressed[last_side], &node_memory_numRBtnPressed[current_side]);
    Counter++;
    node_dt(node_memory_tickHistory[last_side], node_memory_tickHistory[current_side], &node_memory_dt[current_side]);
    Counter++;
    node_velocityX(node_memory_reflectedVelocityX[last_side], node_memory_inputTick[current_side], node_memory_accelX[current_side], node_memory_dt[current_side], node_memory_inputLBtn[current_side], node_memory_lbtn[current_side], node_memory_inputRBtn[current_side], node_memory_rbtn[current_side], &node_memory_velocityX[current_side]);
    Counter++;
    node_velocityY(node_memory_reflectedVelocityY[last_side], node_memory_inputTick[current_side], node_memory_accelY[current_side], node_memory_dt[current_side], node_memory_inputCBtn[current_side], node_memory_cbtn[current_side], &node_memory_velocityY[current_side]);
    Counter++;
    node_reflectedVelocityX(node_memory_velocityX[current_side], node_memory_inputTick[current_side], node_memory_positionX[last_side], &node_memory_reflectedVelocityX[current_side]);
    Counter++;
    node_reflectedVelocityY(node_memory_velocityY[current_side], node_memory_inputTick[current_side], node_memory_positionY[last_side], &node_memory_reflectedVelocityY[current_side]);
    Counter++;
    node_positionX(node_memory_positionX[last_side], node_memory_inputTick[current_side], node_memory_reflectedVelocityX[current_side], node_memory_dt[current_side], &node_memory_positionX[current_side]);
    Counter++;
    node_positionY(node_memory_positionY[last_side], node_memory_inputTick[current_side], node_memory_reflectedVelocityY[current_side], node_memory_dt[current_side], &node_memory_positionY[current_side]);
    Counter++;
    node_ballPosX(node_memory_positionX[current_side], &node_memory_ballPosX[current_side]);
    Counter++;
    node_ballPosY(node_memory_positionY[current_side], &node_memory_ballPosY[current_side]);
    Counter++;
    Output(&node_memory_ballPosX[current_side], &node_memory_ballPosY[current_side], &node_memory_outputDrawBall[current_side], &node_memory_numLBtnPressed[current_side], &node_memory_numCBtnPressed[current_side], &node_memory_numRBtnPressed[current_side], &node_memory_outputNumButtonPressed[current_side]);
    refreshMark();
    current_side ^= 1;
    last_side ^= 1;
  }
}

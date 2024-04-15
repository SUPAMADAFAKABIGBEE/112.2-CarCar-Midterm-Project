/***************************************************************************/
// File       [final_project.ino]
// Author     [Erik Kuo]
// Synopsis   [Code for managing main process]
// Functions  [setup, loop, Search_Mode, Hault_Mode, SetState]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

#define DEBUG  // debug flag

// for RFID
#include "RFID.h"

/*===========================define pin & create module object================================*/
// BlueTooth
// BT connect to Serial1 (Hardware Serial)
// Mega               HC05
// Pin  (Function)    Pin
// 18    TX       ->  RX
// 19    RX       <-  TX
// TB6612, 請按照自己車上的接線寫入腳位(左右不一定要跟註解寫的一樣)
// TODO: 請將腳位寫入下方
#define MotorR_I1 2     // 定義 A1 接腳（右）
#define MotorR_I2 3     // 定義 A2 接腳（右）
#define MotorR_PWMR 11  // 定義 ENA (PWM調速) 接腳
#define MotorL_I3 5     // 定義 B1 接腳（左）
#define MotorL_I4 6     // 定義 B2 接腳（左）
#define MotorL_PWML 12  // 定義 ENB (PWM調速) 接腳
// 循線模組, 請按照自己車上的接線寫入腳位
#define IRpin_LL 32
#define IRpin_L 34
#define IRpin_M 36
#define IRpin_R 38
#define IRpin_RR 40
// RFID, 請按照自己車上的接線寫入腳位
#define RST_PIN 9                 // 讀卡機的重置腳位
#define SS_PIN 53                  // 晶片選擇腳位
RFID* Rfid;  // 建立RFID物件
/*===========================define pin & create module object===========================*/

/*============setup============*/
void setup() {
    // bluetooth initialization
    Serial1.begin(9600);
    // Serial window
    Serial.begin(9600);
    // RFID initial
    SPI.begin();
    Rfid = new RFID(SS_PIN, RST_PIN);
    // TB6612 pin
    pinMode(MotorR_I1, OUTPUT);
    pinMode(MotorR_I2, OUTPUT);
    pinMode(MotorL_I3, OUTPUT);
    pinMode(MotorL_I4, OUTPUT);
    pinMode(MotorL_PWML, OUTPUT);
    pinMode(MotorR_PWMR, OUTPUT);
    // tracking pin
    pinMode(IRpin_LL, INPUT);
    pinMode(IRpin_L, INPUT);
    pinMode(IRpin_M, INPUT);
    pinMode(IRpin_R, INPUT);
    pinMode(IRpin_RR, INPUT);
#ifdef DEBUG
    Serial.println("Start!");
#endif
}
/*============setup============*/

/*=====Import header files=====*/
#include "RFID.h"
#include "bluetooth.h"
#include "node.h"
#include "track.h"
/*=====Import header files=====*/

/*===========================initialize variables===========================*/
int l2 = 0, l1 = 0, m0 = 0, r1 = 0, r2 = 0;  // 紅外線模組的讀值(0->white,1->black)
int vLd = 240;
int vRd = 220;                                // set your own value for motor power
bool state = true;     // set state to false to halt the car, set state to true to activate the car
BT_CMD _cmd = NOTHING;  // enum for bluetooth message, reference in bluetooth.h line 2
/*===========================initialize variables===========================*/

/*===========================declare function prototypes===========================*/
void Search();    // search graph
void SetState();  // switch the state
/*===========================declare function prototypes===========================*/

/*===========================define function===========================*/
void loop() {
    if (!state)
        MotorWriting(0, 0);
    else
        Search();
    SetState();
}

void SetState() {
    // TODO:
    // 1. Get command from bluetooth
    // 2. Change state if need

}

void Search() {
    // TODO: let your car search graph(maze) according to bluetooth command from computer(python
    // code)
    //Serial.println("asking......");
    _cmd = ask_BT();
    switch(_cmd)
    {
        case ADVANCE:
            break;
        case U_TURN:
            halfSpin();
            break;
        case TURN_LEFT:
            leftSpin();
            break;
        case TURN_RIGHT:
            rightSpin();
            break;
        default:
            stop();
            break;
    }
    walk();
}
/*===========================define function===========================*/
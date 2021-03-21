#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>


#define GREEN_LED PB0
#define RED_LED PB1
#define GLOBAL_EN PB2
#define SHUTDOWN_CMD PB3
#define PWR_BUTTON PB4

uint8_t sysState;
uint8_t myTimer;

enum myState {
    INIT = 0, 
    NORMAL, 
    ERROR, 
    SHUTDOWN, 
    OFF};

uint8_t SetTimersPWM();

uint8_t SetLedGreen();
  
uint8_t SetLedRed();

uint8_t SetLedOrange();

uint8_t PwrButtonOn();

uint8_t SetAlarm();

uint8_t SetLedOff();

uint8_t SetAllPorts();

uint8_t ShutdownPulse();

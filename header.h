#include <avr/io.h>
#include <util/delay.h>

#define GREEN_LED PB0
#define RED_LED PB1
#define HEARTBEAT PB2
#define SHUTDOWN_CMD PB3
#define PWR_BUTTON PB4

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
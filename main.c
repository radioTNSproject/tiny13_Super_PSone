#define F_CPU 1000000L
#include "header.h"



int main(void)
{     
    //enum sysState = State;
    uint8_t sysState = INIT;

    while(1)
    {
        switch(sysState) 
        {
            case INIT :
                DDRB |= (1 << GREEN_LED);       // Set pin 6 as output
                DDRB |= (1 << RED_LED);         // Set pin 5 as output
                DDRB |= (1 << SHUTDOWN_CMD);    // Set pin 2 as output

                PORTB &= ~(1 << SHUTDOWN_CMD);  // Maintain Pi off

                SetTimersPWM();                 // Set PWMs
                
                SetLedRed();
                sysState = OFF;
                _delay_ms(2000);
                break;
	
            case NORMAL :
                if (!(PwrButtonOn()))   
                {                    
                    SetLedOrange();
                    sysState = SHUTDOWN;  
                }
                break; 

            case ERROR :
                SetLedRed();
                break; 

            case SHUTDOWN :
                PORTB &= ~(1 << SHUTDOWN_CMD);  // Maintain Pi off
                _delay_ms(2000);
                sysState = OFF;
                break;

            case OFF :
                if (PwrButtonOn())   
                {                    
                    SetLedGreen();
                    PORTB |= (1 << SHUTDOWN_CMD);  // Wake up Pi
                    sysState = NORMAL;  
                }                
                break;
  
            default :
                // wtf
                break;
        }
    }
}



/*
    DDRB |= (1 << DATA);
    DDRB |= (1 << LATCH);
    DDRB |= (1 << CLOCK);
    DDRB |= (1 << LED);

    
    uint8_t test =0;
    test++;
    
    unsigned char sample = 15;
    while(1)
    {
        sample = 0;
        for(int i = 0; i <= 8; i++)
        {
            push8(sample);
            sample = sample*2 + 1; //bar
            //sample++; //incremental
            //sample *= 2; //chenillard
            _delay_ms(500);
            OCR0B = i*8;
            
        }
        
        for(int i = 0; i <= 128; i++)
        {
            OCR0B = i;
            _delay_ms(8);
        }
        for(int i = 128; i != 0; i--)
        {
            OCR0B = i;
            _delay_ms(8);
        }
    }
    return 0;
    

    } //End of while loop
}
*/

uint8_t SetTimersPWM()
{
    TCCR0B |= ((1 << CS01) | (1 << CS00)) ;                      // Set Timer 0 prescaler to clock/8. At 1 MHz this is 125 kHz.        
    TCCR0A |= (1 << WGM01) | (1 << WGM00);      // Set to 'Fast PWM' mode
    TCCR0A |= (1 << COM0B1) | (1 << COM0A1);    // Clear OC0B and OC0A output on compare match, upwards counting.

    return 1;
}

uint8_t SetLedGreen()
{
    OCR0A = 127;
    OCR0B = 1;
    return 1;
}            

uint8_t SetLedRed()
{
    OCR0A = 127;
    OCR0B = 1;
    return 1;
}       

uint8_t SetLedOrange()
{
    OCR0A = 47;
    OCR0B = 127;
    return 1;
}       

uint8_t PwrButtonOn()
{
    // Returns 1 only if PB4 = 0 , button is pressed
    //return (PINB & PB4) ? 0 : 1;  
    if(PINB & (1<<PB4))
    {
        return 0;
    }
    else
    {
        return 1; 
    }
}
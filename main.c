#define F_CPU 1000000L
#include "header.h"

uint8_t SetAllPorts()
{
    DDRB |= (1 << GREEN_LED);
    DDRB |= (1 << RED_LED);
    DDRB |= (1 << SHUTDOWN_CMD);
    DDRB |= (1 << GLOBAL_EN);
    return 1;
}

/* ISR(PCINT0_vect)	     // interrupt service routine
{			             // called when PCINT0 changes state   
   myTimer = 0;
   if (sysState == ERROR)
        sysState = NORMAL;
}

ISR(TIM0_OVF_vect)
{
    myTimer++;
    if (myTimer >= 250)     // Should be after 4sec
        sysState = ERROR; 
} */

uint8_t SetTimersPWM()
{
    TCCR0B |= ((1 << CS01) | (1 << CS00)) ;     // Set Timer 0 prescaler to clock/8. At 1 MHz this is 125 kHz.    
    TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0B1) | (1 << COM0A1) | (1 << COM0B0) | (1 << COM0A0);  // Set to 'Fast PWM' mode, Set OC0B and OC0A output on compare match, upwards counting.
    return 1;
}

uint8_t SetAlarm()              // We are stuck here until reset/shutdown or activity back
{
    while(1)
    {
        SetLedRed();
        _delay_ms(1000);
        SetLedOff();
        _delay_ms(1000);

        if (!(PwrButtonOn()))   
        {                    
            SetLedOrange();
            sysState = SHUTDOWN;  
            _delay_ms(10);
            break;
        }
    }
    return 1;
}


uint8_t SetLedGreen()
{
    OCR0A = 40;
    OCR0B = 0;
    return 1;
}            

uint8_t SetLedRed()
{
    OCR0A = 50;
    OCR0B = 0;
    return 1;
}       

uint8_t SetLedOrange()
{
    OCR0A = 10;
    OCR0B = 50;
    return 1;
}  

uint8_t SetLedOff()
{
    OCR0A = 0;
    OCR0B = 0;
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

uint8_t ShutdownPulse()
{
    PORTB &= ~(1 << SHUTDOWN_CMD);
    _delay_ms(500);
    PORTB |= (1 << SHUTDOWN_CMD);
    return 1;
}


int main(void)
{     
    sysState = INIT;
    myTimer = 0;
    //cli();
    SetAllPorts();

    while(1)
    {
        switch(sysState) 
        {
            case INIT :
                PORTB &= ~(1 << GLOBAL_EN);    // Maintain Pi off
                _delay_ms(10);
                PORTB |= (1 << SHUTDOWN_CMD);  // Shutdown pin inactive

                //PCMSK |= (1<<PCINT2);	    // pin change mask: listen to portb bit 2
	            //GIMSK |= (1<<PCIE);	    // enable PCINT interrupt 
                //TIMSK0 |= (1<<TOIE0);     // enable Timer Overflow interrupt

	            //sei();			        // enable all interrupts

                SetTimersPWM();             // Set PWMs freq and values
                
                SetLedOrange();
                sysState = OFF;
                break;
	
            case NORMAL :
                if (!(PwrButtonOn()))
                {                    
                    SetLedOrange();
                    sysState = SHUTDOWN;  
                    _delay_ms(10);
                }
                else
                {
                    _delay_ms(10);
                }
                break; 

            case ERROR :                    // abandonned
                SetAlarm();
                break; 

            case SHUTDOWN :
                ShutdownPulse();                // Shutdown command via GPIO3
                _delay_ms(10000);               // waiting total shutdown
                PORTB &= ~(1 << GLOBAL_EN);     // Maintain Pi off                
                sysState = OFF;
                //cli();                        // Stop interrupts, no need to check bpm or timer
                break;

            case OFF :
                if (PwrButtonOn())
                {                    
                    SetLedGreen();
                    PORTB |= (1 << GLOBAL_EN);  // Release power supply
                    sysState = NORMAL; 
                    //sei();                    // Start interrupts
                    _delay_ms(10); 
                }
                else
                {
                    _delay_ms(10); 
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

pseudo code:
PCMSK = (1 << PCINT3) | (1 << any other PCINTx, etc.) - select which pin or pins trigger PCINT0
GIFR |= (1 << PCIF) - clear any already pending PCINT0 interrupt flag
GIMSK |= (1 << PCIE) - enable the pin change interrupt
Don't forget the SEI


ISR(PCINT0_vect)	     // interrupt service routine
{			     // called when PCINT0 changes state
   PORTB = (PORTB ^ 0x16);   // toggle red led, portb bit 5, pin 3
   return;
}

void SystemInit(void)
{
	PCMSK |= (1<<PCINT1);	// pin change mask: listen to portb bit 2
	GIMSK |= (1<<PCIE);	// enable PCINT interrupt 
	sei();			// enable all interrupts
}

*/


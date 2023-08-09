#include <msp430.h> 
#include <stdint.h>
#include <stdlib.h>
#include "libs/servomotor.h"
#include "libs/sensor.h"

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer.

    // Configure everything needed.
    sensorConfig();
    servoConfig();

    // Start LEDs as output and turned off.
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;

    __enable_interrupt();       // Enable interruptions

    volatile float distance;

    while(1){
        distance = sensorTriggerPulse();    // Measure distance with HC-SR04
        __delay_cycles(5000);               // Wait 50ms to reduce interference between servo and HC-SR04
        if (distance <= 10){
            P1OUT |= BIT0;
            P4OUT |= BIT7;
            servoSetPosition(90);           // Turn on water font.
        }
        else{
            P1OUT &= ~BIT0;
            P4OUT &= ~BIT7;
            servoSetPosition(0);            // Turn off water font.
        }
    }
}

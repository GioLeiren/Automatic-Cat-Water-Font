#include <msp430.h>
#include <stdint.h>
#include "sensor.h"

void sensorConfig(){
    P1DIR |= BIT4;              // Pin 1.4 (trigger) as output.
    P1DIR &= ~BIT5;             // Pin 1.5 (echo) as input.
    P1SEL |= BIT5;              // Pin 1.5 (echo) for capture timer.

    P1IE |= BIT5;               // Enable interruption for pin 1.5 (echo).
    P1IES &= ~BIT5;             // Interruption at rising edge.
    P1IFG &= ~BIT5;             // Clear interruption flag.
}

float sensorTriggerPulse(){
    {
        TA2CTL = TASSEL_2 | ID_0 | MC_1 | TACLR;
        TA2CCTL1 = CM_3 | CCIS_0 | CAP | OUTMOD_7 | CCIE;
        TA2CCR0 = 20000 - 1;
        TA2CCR1 = 600;

        // Send trigger signal
        P1OUT |= BIT4;
        __delay_cycles(10);
        P1OUT &= ~BIT4;

        // Wait echo signal
        while(!(P1IN & BIT5));
        unsigned int start_time = TA2R;
        while((P1IN & BIT5));
        unsigned int end_time = TA2R;
        unsigned int duration = end_time - start_time;

        // Convert duration to distance in centimeters
        float distance = duration / 58.0;

        return distance;

    }
}


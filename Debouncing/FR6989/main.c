#include <msp430.h>

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;           //stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           //Disable the GPIO power-on default high-impedance mode
 
    P1DIR |= BIT0;                      //set direction to output
    P1OUT &= ~BIT0;                     //turn off LED
    P1DIR &= ~BIT1;                     //set direction to input
    P1REN |= BIT1;                      //enable pull up resistor
    P1IE |= BIT1;                       //enable interrupt
    P1IES |= ~BIT1;                     //Hi/Lo edge trigger
    P1IFG &= ~BIT1;                     //clear interrupt flag

    TA0CCR0 = 100;                       //debounce time
    TA0CCTL0 |= CCIE;                   //enable interrupts
    TA0CTL |= TASSEL_1 + MC_2;          //aclk, continuous mode

    __bis_SR_register(LPM0 + GIE);      //enable low power mode and interrupts

}
#pragma vector=TIMER0_A0_VECTOR         //interrupt protocol and memory allocation
__interrupt void Timerinter(void)      //interrupt name
{
    TA0CTL &= ~TAIFG;                   //clear interrupt flag
    TA0CTL |= MC_0;                     //stop timer
    if(!(P1IN & BIT1))                  //if button is pressed
        P1OUT ^= BIT0;                  //toggle LED
}
#pragma vector = PORT1_VECTOR           //interrupt protocol and memory allocation
__interrupt void Buttonpressed(void)      //interrupt name
{
    P1IFG &= ~BIT1;                     //clear interrupt flag
    TA0R = 0;                           //reset timer
    TA0CTL |= MC_2;                     //start continuous mode
}

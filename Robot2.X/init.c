#include <xc.h>

////  BITS DE CONFIGURATION
/////////////////////////////////////////////////////////////////////////////////
//// disables the JTAG to prevent conflicts with TRISA
//_FICD(ICS_PGD1 & JTAGEN_OFF);

//// Select internal FRC oscillator as clock source
//_FOSCSEL(FNOSC_FRC);
//// enables clock switching and configure the primary oscillator for a 10MHz crystal
//_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);

#pragma config OSCIOFNC = ON        // Oscillator Configuration (clock switching: disabled;
#pragma config FCKSM = CSDCMD       // failsafe clock monitor: disabled; OSC2 pin function: digital IO;
#pragma config POSCMD = NONE        // primary oscillator mode: disabled)

#pragma config FNOSC = FRCPLL       // Oscillator Selection PLL

//#pragma config FNOSC = FRC        // Oscillator Selection no PLL

#pragma config FWDTEN = OFF         // Turn off WatchDog Timer
#pragma config GWRP = OFF			// Turn off code protect
#pragma config FPWRT = 0			// Turn off power up timer


void oscillatorInit(void) {
    PLLFBD = 38; // M=40
    __builtin_write_OSCCONH( 3 );
    __builtin_write_OSCCONL( 1 );
//    // Wait for Clock switch to occur
//    while (OSCCONbits.COSC != 0b011);
    
    //	PLL setup:
	CLKDIVbits.PLLPRE = 0;          // N1=2: PLL VCO Output Divider Select bits; 0 -> /2 (default)
	PLLFBDbits.PLLDIV = 42 - 2;     // M=40: PLL Feedback Divisor bits; 42 - 2 = 40 -> 40 x multiplier
                                    // (divisor is 2 more than the value)
    CLKDIVbits.PLLPOST = 0;         // N2=2: PLL Phase Detector Input Divider bits; 0 -> /2	
    while(OSCCONbits.LOCK != 1);	// Wait for PLL to lock
    //while (OSCCONbits.COSC != 0b011);
}



//#include <xc.h>
//
////  BITS DE CONFIGURATION
/////////////////////////////////////////////////////////////////////////////////
//// disables the JTAG to prevent conflicts with TRISA
//_FICD(ICS_PGD1 & JTAGEN_OFF);
//
//// disables the watchdog
//_FWDT(FWDTEN_OFF);
//
//// The dsPIC will be clocked by the primary oscillator with a 10MHz crystal.
//// We want to use the PLL to obtain Fosc = 80MHz ( <=> 40MIPS ).
//// Problem : with a 10MHz crystal, PLL constraints are not met with the
//// default parameter.
//// Solution :
////	- boot using the internal FRC oscillator as clock source,
////	- set the right PLL parameters to obtain Fosc = 80MHz, without violating
////	  the PLL constraints,
////	- switch the clock source to the PLL
////	- wait for the end of the clock switch
////
//// Select internal FRC oscillator as clock source
//_FOSCSEL(FNOSC_FRC);
//// enables clock switching and configure the primary oscillator for a 10MHz crystal
//_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//
//
//void oscillatorInit(void) {
//    PLLFBD = 38; // M=40
//    CLKDIVbits.PLLPOST = 0; // N1=2
//    CLKDIVbits.PLLPRE = 0; // N2=2
//    __builtin_write_OSCCONH( 3 );
//    __builtin_write_OSCCONL( 1 );
//    // Wait for Clock switch to occur
//    while (OSCCONbits.COSC != 0b011);
//}



#include "motor.h"

void timerInit(void) {
//    INTCON1bits.NSTDIS = 1;
//    IEC0bits.T1IE = 1;
//    IEC0bits.T2IE = 1;
//    IEC0bits.T3IE = 1;
    
    
    T1CONbits.TON = 0;
    T1CONbits.TCS = 0;
    T1CONbits.TCKPS = 0b10;
    TMR1 = 0;
    PR1 = 6249;             // T = 10 ms
       
    T2CONbits.TON = 0;
    T2CONbits.TCS = 0;
    T2CONbits.TCKPS = 0b10;
    TMR2 = 0;
    PR2 = 6249;            // T = 10 ms
    
    T3CONbits.TON = 0;
    T3CONbits.TCS = 0;
    T3CONbits.TCKPS = 0b10;
    TMR3 = 0;
    PR3 = 12499;           // T
    
    
    //T1CONbits.TON = 1;
    T2CONbits.TON = 1;
    T3CONbits.TON = 1;
}

//void _ISR __attribute__((no_auto_psv)) _T1Interrupt(void) {
//    IFS0bits.T1IF = 0;
//    dutyCycleMotorRight(0);
//}
//
//void _ISR __attribute__((no_auto_psv)) _T2Interrupt(void) {
//    IFS0bits.T2IF = 0;
//    dutyCycleMotorLeft(0);
//}
//
//void _ISR __attribute__((no_auto_psv)) _T3Interrupt(void) {
//    IFS0bits.T3IF = 0;
//    
//    dutyCycleMotorRight(30);
//    TMR1 = 0;
//
//    dutyCycleMotorLeft(30);
//    TMR2 = 0;
//}
#include "xc.h"
#include "init.h"

// disables the JTAG to prevent conflicts with TRISA
_FICD(JTAGEN_OFF);
// disables the watchdog
_FWDT(FWDTEN_OFF);
// ???
_FGS(GSS_OFF & GCP_OFF & GWRP_OFF);

// The dsPIC will be clocked by the primary oscillator with a 10MHz crystal.
// We want to use the PLL to obtain Fosc = 80MHz ( <=> 40MIPS ).
// Problem : with a 10MHz crystal, PLL constraints are not met with the
// default parameter.
// Solution :
//	- boot using the internal FRC oscillator as clock source,
//	- set the right PLL parameters to obtain Fosc = 80MHz, without violating
//	  the PLL constraints,
//	- switch the clock source to the PLL
//	- wait for the end of the clock switch
//
// Select internal FRC oscillator as clock source
_FOSCSEL(FNOSC_FRC);
// enables clock switching and configure the primary oscillator for a 10MHz crystal
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);



void oscillatorInit(void) {
	// Configures PLL prescaler, PLL postscaler, PLL divisor
	// to obtain Fosc = 80MHz with the 10MHz xt oscillator
	// Fosc = Fin*M/(N1+N2), where :
	// 		N1 = PLLPRE + 2
	// 		N2 = 2 x (PLLPOST + 1)
	//		M = PLLDIV + 2
	PLLFBD = 30;						// M = 32
	CLKDIVbits.PLLPRE = 0;				// N1 = 2
	CLKDIVbits.PLLPOST = 0;				// N2 = 2
	// Initiate Clock Switch to Primary Oscillator with PLL
	__builtin_write_OSCCONH( 3 );
	__builtin_write_OSCCONL( 1 );
	// Wait for Clock switch to occur
	while (OSCCONbits.COSC != 0b011);
}

void UART1Init() {
    U1MODEbits.PDSEL = 00;              // 8-bit data, no parity
    U1BRG = (FCY/(16*BAUDRATE)) - 1;    // Configuaration du baud rate
    U1MODEbits.UARTEN = 1;              // Activation de la réception
	U1STAbits.UTXEN = 1;                // Activation de la transmission
    
    U2MODEbits.PDSEL = 00;              // 8-bit data, no parity
    U2BRG = (FCY/(16*BAUDRATE)) - 1;    // Configuaration du baud rate
    U2MODEbits.UARTEN = 1;              // Activation de la réception
	U2STAbits.UTXEN = 1;                // Activation de la transmission
    
    //---ASSIGNATION DES PINS---    
    RPOR3bits.RP7R = 3;    //TX PIN ORDRE
    RPOR3bits.RP6R = 5;    //TX PIN PARAM
}

void timer3Init(void) {                 //Timer de l'ADC 
    T3CONbits.TON = 0;
    T3CONbits.TCS = 0;
    T3CONbits.TCKPS=0;
    TMR3=0;
    PR3 = 2856;
    T3CONbits.TON = 1;
}

void adcInit(void) {
    TRISAbits.TRISA0 = 1;
    
    AD1CON1bits.AD12B = 0;  // Convertisseur sur 10 bits
    AD1CON3bits.ADCS = 5;   // Clock de l'adc
    AD1CON1bits.ASAM = 1;   // auto sample activé
    AD1CSSLbits.CSS0 = 1;   // Le convertisseur doit scanner la patte AN0
    AD1PCFGLbits.PCFG0 = 0; // AN0 en mode analogique
    AD1CON1bits.SSRC = 0b010;// ADC cadencé par le Timer3
    IFS0bits.AD1IF = 0;		//reset du flag d'interruption
    AD1CON1bits.ADON = 1;   // l'ADC est actif
}

void initAll() {
    oscillatorInit();
    UART1Init();
    timer3Init(); 
    adcInit();
}
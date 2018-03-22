#include <xc.h>
#include "UART.h"

void UARTInit() {
	U1MODEbits.UARTEN = 1;              // Activation de la réception
	U1STAbits.UTXEN = 1;                // Activation de la transmission
    U1MODEbits.PDSEL = 00;              // 8-bit data, no parity
    U1BRG = (FCY/(16*BAUDRATE)) - 1;    // Configuaration du baud rate
    
    U2MODEbits.UARTEN = 1;              // Activation de la réception
	U2STAbits.UTXEN = 1;                // Activation de la transmission
    U2MODEbits.PDSEL = 00;              // 8-bit data, no parity
    U2BRG = (FCY/(16*BAUDRATE)) - 1;    // Configuaration du baud rate
    
    RPINR18bits.U1RXR = 10; // RX PIN
    RPINR19bits.U2RXR = 11; // RX PIN
//    RPOR5bits.RP11R = 3;    // TX PIN 
//    RPOR5bits.RP10R = 5;    // TX PIN 
//    U1STAbits.URXISEL = 0b10;
//    IEC0bits.U1RXIE = 1;
}

void echo(){
    // teste la communication série
    if(U1STAbits.URXDA){
        U1TXREG = U1RXREG;
    }
}

int readRx(){
    return U1RXREG;
}

void sendTx(float data) {
    //convertie les données en cm et les envoie
    data *= 100;
    U1TXREG = (int) data;
}

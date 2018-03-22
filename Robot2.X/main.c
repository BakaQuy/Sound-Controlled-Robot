#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "init.h"
#include "motor.h"
#include "encoder.h"
#include "UART.h"
#include "regulation.h"

int main() {
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    
    LATBbits.LATB4 = 1;
    LATBbits.LATB5 = 0;
    
    oscillatorInit();
    timerInit();
    motorInit();
    encoderInit();
    UARTInit();
    
    int order;
    int param;

    
    while(1) {  
    	//  l'ordre est lu dans le canal U1RXREG
        if(U1STAbits.URXDA){
            order = U1RXREG;
        }
        // le paramètre est lu dans le canal U2RXREG
        if(U2STAbits.URXDA){
            param = U2RXREG;
            if (param != 0) {
            	// envoi de la commande au robot
                go(order, param);
            }
        }
    }
    return 0;
}

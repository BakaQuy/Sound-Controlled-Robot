/* 
 * File:   main.c
 * Author: Quy Phan
 *
 * Created on 7 mai 2017, 20:01
 */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "init.h"
#include "digital_filter.h"
#include "FskDetector.h"


int main() {
    initAll();
    int sample, message, detLow, detHigh;
    
    TRISBbits.TRISB8 = 0;  //1100 Hz
    TRISBbits.TRISB9 = 0;  //900 Hz
    TRISBbits.TRISB10 = 0; //message reçu
    
    LATBbits.LATB8 = 0;
    LATBbits.LATB9 = 0;
    LATBbits.LATB10 = 1;
    
    while(1){
        if(IFS0bits.AD1IF == 1){
            IFS0bits.AD1IF = 0;
            sample = ADC1BUF0;
            filter(sample);                                                     //filtre l'échantillon entrant avec les 2 filtres numériques
            detLow = detect900();                                               //détection fréquence 900Hz
            detHigh = detect1100();                                             //détection fréquence 1100Hz 
            message = fskDetector(detLow, detHigh);                             //décodage du message
            if(message){
               LATBbits.LATB10 = !PORTBbits.RB10;                               //Change l'état de la LED si le message est bien reçu
               U1TXREG = (unsigned char)(message >> 8);                         //décalage binaire pour envoyer que l'ordre
               U2TXREG = (unsigned char)(message & 0x00FF);                     //envoie que le paramètre en enlevant les bits d'ordre        
            }
        }
    }
    return 0;
}
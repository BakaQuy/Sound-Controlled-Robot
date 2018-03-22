#include <xc.h>
int pos1Init;
int pos2Init;

float posRight;
float posLeft;

float velocityRight;
float velocityLeft;

void encoderInit(void){
//    ---RIGHT MOTOR---
    TRISCbits.TRISC8 = 1;
    TRISCbits.TRISC9 = 1;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    
    
    RPINR14bits.QEA1R = 25;     //RP24
    RPINR14bits.QEB1R = 24;     //RP25
    
//    ---LEFT MOTOR---
    RPINR16bits.QEA2R = 19;      //RP19
    RPINR16bits.QEB2R = 20;      //RP20
    
    QEI1CONbits.QEIM = 0b111;
    QEI2CONbits.QEIM = 0b111;       
    
//  Registre des positions
    POS1CNT = 30000;
    POS2CNT = 30000;
    
    pos1Init = 30000;
    pos2Init = 30000;

    posRight = 0;
    posLeft = 0;

    velocityRight = 0;
    velocityLeft = 0;
}

void resetPos(void){
    POS1CNT = 30000;
    POS2CNT = 30000;
}

float getPosRight(void){
   //evalue la position réelle de la roue droite apd des encodeurs
   return 0.0508*(3.1415/180.0)*((float)POS1CNT-pos1Init); 
}

float getPosLeft(void){
   //evalue la position réelle de la roue gauche apd des encodeurs
   return 0.0508*(3.1415/180.0)*((float)POS2CNT-pos2Init); 
}

void getVelocity(void) {
    // on releve les positions
    float currentPosRight = getPosRight();
    float currentPosLeft = getPosLeft();
    
    // on attend 10 ms
    TMR1 = 0;
    T1CONbits.TON = 1;
    while(!IFS0bits.T1IF) {}
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 0;
    // on releve les positions à nouveaux
    velocityRight = (currentPosRight - posRight)/0.01;
    velocityLeft = (currentPosLeft - posLeft)/0.01;
    
    posRight = currentPosRight;
    posLeft = currentPosLeft;
}

float getVelocityRight(void){
    return velocityRight;
}

float getVelocityLeft(void){
    return velocityLeft;
}
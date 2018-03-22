#include <xc.h>

void motorInit(void){
    // OCxCON : registre de contrôle du périphérique.
    // OCxR : registre de comparaison, sa valeur définit le rapport cyclique du signal de sortie.
    // OCxRS : registre secondaire de comparaison, il sert à synchroniser l'écriture dans OCxR avec le débordement de TMRy.
    
    RPOR6bits.RP13R = 0b10011;  // RIGHT WHEEL OC2
    RPOR9bits.RP18R = 0b10100;  // LEFT WHEEL OC3
    
//  ---RIGHT MOTOR---
    OC2CONbits.OCM = 0b000;
    OC2CONbits.OCTSEL = 0;
    OC2R = 0;
    OC2RS = 0;
    OC2CONbits.OCM = 0b110;
    
//  ---LEFT MOTOR---
    OC3CONbits.OCM = 0b000;
    OC3CONbits.OCTSEL = 0;
    OC3R = 0;
    OC3RS = 0;
    OC3CONbits.OCM = 0b110;
}
    
void velocityMotors(float velocityR, float velocityL){
    float dcR;
    float dcL;
    
    // converti la vitesse en duty cycle
    if (velocityR > 0) {
        dcR = (velocityR/0.91)+0.23;  
    } else if (velocityR < 0) {
        dcR = (velocityR/0.91)-0.31;  
    } else {
        dcR = 0;
    }
    
    if (velocityL > 0) {
        dcL = (velocityL/0.86)+0.3;
    } else if (velocityL < 0){
        dcL = (velocityL/0.86)-0.26;
    } else {
        dcL = 0;
    }

    // commande les moteurs après avoir calculé le dc
    dc(dcR, dcL);
}

void dc(float dcR, float dcL) {
    OC3RS = ((1.5 - (dcL*0.5))/10.0)*(PR2+1);
    OC2RS = ((1.5 + (dcR*0.5))/10.0)*(PR2+1);
}
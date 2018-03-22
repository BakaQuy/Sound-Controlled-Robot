#include <xc.h>
#include "regulation.h"
#include "encoder.h"
#include "motor.h"
#include <math.h>
#include "UART.h"


void go(int order, int param) {
    float dist;
    int direction;
    int param_signed;
    
    resetPos();
    if (order == 0) {
        direction = (unsigned char)(param >> 7);
        param_signed = param & 0b01111111;
        if (direction == 0){
            dist = ((float)(param_signed))/100.0;
        } else if (direction == 1){
            dist = -(((float)(param_signed))/100.0);
        }
        regulatePosition(dist, dist); // go forward
    }
    else if (order == 1) {
        dist = (float)param*(3.1415/180)*Radius; // angle to length
        regulatePosition(dist, -dist); // turn right
    }
    else if (order == 2) {
        dist = (float)param*(3.1415/180)*Radius; // angle to length
        regulatePosition(-dist, dist); // turn left
    }
}

void delay() {
    TMR1 = 0;
    T1CONbits.TON = 1;
    while(!IFS0bits.T1IF) {}
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 0;
}

void delay_ms(int ms){
    PR4 = 625; //valeur de PR pour avoir 1ms avec un prescaler de 64
    T4CONbits.TCKPS = 0b10; // x64 prescaler
    T4CONbits.TON = 1; // le timer est lancé
    while(ms > 0){ // tant que ms est positif
        while(IFS1bits.T4IF != 1); // on attend que le flag s'active
        IFS1bits.T4IF = 0; // on annule flag
        ms--; // on décrémente ms jusqu'à ce qu'il soit nul
    }
}


void polar(float speedR, float speedL, float posR, float posL) {
    float pR = 7;
    float pL = 7;
    
    if (posL < 0) {
        pL = - pL;
    }
    if (posR < 0) {
        pR = - pR;
    }
    
    float averagePos = (fabs(posR) + fabs(posL))/2;
    float errorR = averagePos - fabs(posR);
    float errorL = averagePos - fabs(posL);
    
    velocityMotors(speedR + pR*errorR, speedL + pL*errorL);
    
}


void regulatePosition(float inputPosR, float inputPosL) {
    float VMAX = 0.3;
    float AMAX = 0.5;
    int COUNTER = 0;
    int TICK = 0;

    float speedR;
    float speedL;
    
    float oldSpeedR = 0;
    float oldSpeedL = 0;
    
    float oldPosR = 0;
    float oldPosL = 0;
    
    float currentPosR = 0;
    float currentPosL = 0;
    
    float errorPosR = inputPosR - currentPosR;;
    float errorPosL = inputPosL - currentPosL;
    
    float errorPosRelR = errorPosR/inputPosR;
    float errorPosRelL = errorPosL/inputPosL;
    
    while (1) {
        
        speedR = KpR*errorPosR;
        speedL = KpL*errorPosL;

        // speed saturation Right
        if(speedR > VMAX) {
            speedR = VMAX;
        } else if (speedR < -VMAX) {
            speedR = -VMAX;
        }
        // speed saturation Left
        if(speedL > VMAX) {
            speedL = VMAX;
        } else if (speedL < -VMAX) {
            speedL = -VMAX;
        }
        
        // acceleration saturation Right
        if (speedR - oldSpeedR > AMAX*deltaT) {
            speedR = oldSpeedR + AMAX*deltaT;
        } 
        else if (speedR - oldSpeedR < -AMAX*deltaT) {
            speedR = oldSpeedR - AMAX*deltaT;
        }
        // acceleration saturation Left
        if (speedL - oldSpeedL > AMAX*deltaT) {
            speedL = oldSpeedL + AMAX*deltaT;
        } 
        else if (speedL - oldSpeedL < -AMAX*deltaT) {
            speedL = oldSpeedL - AMAX*deltaT;
        }
        
        oldSpeedR = speedR;
        oldSpeedL = speedL;

        //polar(speedR, speedL, currentPosR, currentPosL);
        velocityMotors(speedR,speedL);
        oldPosR = currentPosR;
        oldPosL = currentPosL;
        
        currentPosR = getPosRight();
        currentPosL = getPosLeft();
        
        errorPosR = inputPosR - currentPosR;
        errorPosL = inputPosL - currentPosL;
        
        errorPosRelR = errorPosR/inputPosR;
        errorPosRelL = errorPosL/inputPosL;
        
        if (sqrt(pow(errorPosR,2) + pow(errorPosL,2)) < 0.2) {
            VMAX = 0.15;
        }
        
        if (sqrt(pow(errorPosRelR,2) + pow(errorPosRelL,2)) < 0.2) {
            TICK += 1;
            if (TICK >= 500) {
                velocityMotors(0,0);
                break;
            }
        }

        if (sqrt(pow(errorPosRelR,2) + pow(errorPosRelL,2)) < 0.05) {
            velocityMotors(0,0);
        }
        
        delay_ms(10);
    }
}


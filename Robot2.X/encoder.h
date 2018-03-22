/* 
 * File:   encoder.h
 * Author: Quy Phan
 *
 * Created on 12 mars 2017, 14:43
 */

#ifndef ENCODER_H
#define	ENCODER_H

void encoderInit(void);

void resetPos(void);

float getPosRight();
float getPosLeft();

void getVelocity(void);
float getVelocityRight(void);
float getVelocityLeft(void);

#endif	/* ENCODER_H */


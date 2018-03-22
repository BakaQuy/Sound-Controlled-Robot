/* 
 * File:   regulation.h
 * Author: vador
 *
 * Created on 21 mars 2017, 14:43
 */

#ifndef REGULATION_H
#define	REGULATION_H

#define deltaT 0.01 // s, periode echantillonnage

//#define KpR 4.65//8.49 // m?¹
//#define KpL 4.95
#define KpR 3.92//8.49 // m?¹
#define KpL 4.73
#define Radius 0.1075 // m, distance_between_wheels/2

void go(int, int);
void resetErrors(void);
void regulatePosition(float, float);
void delay(void);
void delay_ms(int);
void polar(float, float, float, float);

#endif	/* REGULATION_H */


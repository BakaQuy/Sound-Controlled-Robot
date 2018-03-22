/* 
 * File:   init.h
 * Author: Quy Phan
 *
 * Created on 7 mai 2017, 19:57
 */

#ifndef INIT_H
#define	INIT_H

#define FCY 40000000 // fréquence machine

#define FCY 40000000
#define BAUDRATE 115200

/**
 * Configure le dsPIC pour utiliser l'oscillateur
 */
void oscillatorInit(void);

/**
 * Configure l'UART pour la communication série
 */
void UART1Init();

/**
 * Configure le TIMER3 utilisé pour l'ADC (échantillonnage à 14 kHz)
 */
void timer3Init(void);

/**
 * Configure l'adc cadencé au Timer3
 */
void adcInit(void);

/**
 * Initialise le timer, l'oscillator, l'UART et l'adc
 */
void initAll();
#endif	/* INIT_H */


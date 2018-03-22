/* 
 * File:   UART.h
 * Author: Quy Phan
 *
 * Created on 20 février 2017, 20:02
 */

#ifndef UART_H
#define	UART_H

#define BAUDRATE 115200
#define FCY 40000000 // fréquence machine

/**
 * Configure le TIMER3 utilisé pour l'ADC
 */
void UARTInit();

/**
 * Renvoie l'octet reçu
 */
void echo(void);

int readRx(void);

void sendTx(float);

#endif	/* UART_H */


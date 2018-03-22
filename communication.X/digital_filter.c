#include <xc.h>
#include "digital_filter.h"

#define M 4
#define N 6

#define SEUIL900 80                                                             //Seuil de détection pour détecteur de crête 900 Hz
#define SEUIL1100 90                                                            //Seuil de détection pour détecteur de crête 1100 Hz

const long SOS900[M][N] =  {{513, 0, -513, 65536, -119780, 65136},              //Matrice SOS pour 900 Hz
                            {513, 0, -513, 65536, -120540, 65150},
                            {511, 0, -511, 65536, -119498, 64585},
                            {511, 0, -511, 65536, -119819, 64598}};

const long SOS1100[M][N] = {{627, 0, -627, 65536, -114439, 65048},              //Matrice SOS pour 1100 Hz
                            {627, 0, -627, 65536, -115547, 65064},
                            {624, 0, -624, 65536, -114177, 64375},
                            {624, 0, -624, 65536, -114642, 64391}};

static long p900[5][3]  =  {{0,0,0},                                            //Matrice pour stocker valeur pour le filtre 900Hz
                            {0,0,0},
                            {0,0,0},
                            {0,0,0},
                            {0,0,0}};

static long p1100[5][3] =  {{0,0,0},                                            //Matrice pour stocker valeur pour le filtre 1100Hz
                            {0,0,0},
                            {0,0,0},
                            {0,0,0},
                            {0,0,0}};

static int samples900[15] = {0};                                                //Conserve 15 dernier échantillons pour le détecteur de crête 900Hz
static int samples1100[10]= {0};                                                //Conserve 10 dernier échantillons pour le détecteur de crête 1100Hz

void filter(int sample){
    
    //900 Hz
    p900[0][2] = p900[0][1];
    p900[0][1] = p900[0][0];
    p900[0][0] = sample;
 
    //étage 0:
    p900[1][2]= p900[1][1];
    p900[1][1]= p900[1][0];
    p900[1][0]= ((SOS900[0][0]*p900[0][0] + SOS900[0][1]*p900[0][1] + SOS900[0][2]*p900[0][2]) - SOS900[0][4]*p900[1][1] - SOS900[0][5]*p900[1][2])>>16;
    
    //étage 1
    p900[2][2]= p900[2][1];
    p900[2][1]= p900[2][0];
    p900[2][0]= ((SOS900[1][0]*p900[1][0] + SOS900[1][1]*p900[1][1] + SOS900[1][2]*p900[1][2]) - SOS900[1][4]*p900[2][1] - SOS900[1][5]*p900[2][2])>>16;
    
    //étage 2 
    p900[3][2]= p900[3][1];
    p900[3][1]= p900[3][0];
    p900[3][0]= ((SOS900[2][0]*p900[2][0] + SOS900[2][1]*p900[2][1] + SOS900[2][2]*p900[2][2]) - SOS900[2][4]*p900[3][1] - SOS900[2][5]*p900[3][2])>>16;
    
    //étage 3
    p900[4][2]= p900[4][1];
    p900[4][1]= p900[4][0];
    p900[4][0]= ((SOS900[3][0]*p900[3][0] + SOS900[3][1]*p900[3][1] + SOS900[3][2]*p900[3][2]) - SOS900[3][4]*p900[4][1] - SOS900[3][5]*p900[4][2])>>16;
    
    //U1TXREG = (int) ((p900[4][0]+512)/4);
    
    
    //1100 Hz
    p1100[0][2] = p1100[0][1];
    p1100[0][1] = p1100[0][0];
    p1100[0][0] = sample;
 
    //étage 0:
    p1100[1][2]= p1100[1][1];
    p1100[1][1]= p1100[1][0];
    p1100[1][0]= ((SOS1100[0][0]*p1100[0][0] + SOS1100[0][1]*p1100[0][1] + SOS1100[0][2]*p1100[0][2]) - SOS1100[0][4]*p1100[1][1] - SOS1100[0][5]*p1100[1][2])>>16;
    
    //étage 1
    p1100[2][2]= p1100[2][1];
    p1100[2][1]= p1100[2][0];
    p1100[2][0]= ((SOS1100[1][0]*p1100[1][0] + SOS1100[1][1]*p1100[1][1] + SOS1100[1][2]*p1100[1][2]) - SOS1100[1][4]*p1100[2][1] - SOS1100[1][5]*p1100[2][2])>>16;
    
    //étage 2 
    p1100[3][2]= p1100[3][1];
    p1100[3][1]= p1100[3][0];
    p1100[3][0]= ((SOS1100[2][0]*p1100[2][0] + SOS1100[2][1]*p1100[2][1] + SOS1100[2][2]*p1100[2][2]) - SOS1100[2][4]*p1100[3][1] - SOS1100[2][5]*p1100[3][2])>>16;
    
    //étage 3
    p1100[4][2]= p1100[4][1];
    p1100[4][1]= p1100[4][0];
    p1100[4][0]= ((SOS1100[3][0]*p1100[3][0] + SOS1100[3][1]*p1100[3][1] + SOS1100[3][2]*p1100[3][2]) - SOS1100[3][4]*p1100[4][1] - SOS1100[3][5]*p1100[4][2])>>16;
    
    //U1TXREG = (int) ((p1100[4][0]+512)/4);
}

int detect900(){                                                                //Détecteur de crête 900Hz                                                               
    int n;
    int max = 0;                                                                //Valeur max du signal en entrée
    int min = 0;                                                                //Valeur min du signal en entrée
    int res;
    for(n = 14; n>0; n--){                                                      
        samples900[n] = samples900[n-1];                                        //Décalage des échantillons dans le tableau de données
        if(samples900[n] > max){                                                //Stocke la valeur maximal du signal en entrée
            max = samples900[n];
        }
        if(samples900[n] < min){
            min = samples900[n];                                                //Stocke la valeur minimal du signal en entrée
        }
    }
    samples900[0] = p900[4][0];
    if(samples900[0] > max){                                                    //Compare la valeur maximale avec l'échantillon entrant 
        max = samples900[0];
    }
    if(samples900[0] < min){ 
        min = samples900[0];                                                    //Compare la valeur minimal avec l'échantillon entrant 
    }
    if(max-min > SEUIL900){                                                     //Renvoie 1 si la valeur crête à crête est supérieure au SEUIL et allume une LED
        LATBbits.LATB9 = 1;
        res = 1;
    }
    else{                                                                       //Renvoie 0 si la valeur crête à crête est inférieure au SEUIL
        LATBbits.LATB9 = 0;
        res = 0; 
    }
    return res;
}

int detect1100(){                                                               //Détecteur de crête 1100Hz   (même algorithme que 900 Hz)
    int n;
    int max = 0; 
    int min = 0;
    int res;
    for(n = 9; n>0; n--){
        samples1100[n] = samples1100[n-1];                                       
        if(samples1100[n] > max){
            max = samples1100[n];
        }
        if(samples1100[n] < min){
            min = samples1100[n];
        }
    }
    samples1100[0] = p1100[4][0];
    if(samples1100[0] > max){
        max = samples1100[0];
    }
    if(samples1100[0] < min){
        min = samples1100[0];
    }
    if(max-min > SEUIL1100){
        res = 1;
        LATBbits.LATB8 = 1;
    }
    else{
        LATBbits.LATB8 = 0;
        res = 0;
    }
    return res;
}
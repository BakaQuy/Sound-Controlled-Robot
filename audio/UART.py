# -*- coding: cp1252 -*-
import sys                      # pour sys.exit
import serial                   # package de gestion du port serie
from struct import unpack       # pour decoder le "bytearray" re�u
import numpy                    # pour creer les vecteurs
from matplotlib import pyplot   # pour l'affichage
import time


# Configuration du port serie
serialPort = serial.Serial("/dev/ttyUSB0", baudrate=115200)    # creation de l'objet associe au port serie
# serialPort.baudrate = 115200    # definition du baudrate
# serialPort.port = 'COM5'        # selection du port serie

# Connexion au dsPIC
try:
    print(serialPort.name)
    serialPort.open()               # on tente d'etablir la connexion
except:
    print('Echec de la connexion')  # En cas de probleme,
    sys.exit()                   # on ferme l'application

# Parametres de l'acquisition
sampleNb = 1000                     # Nombre d'echantillons a lire
samplePer = 100E-3 #200E-6                  # Periode d'echantillonnage

# Creation d'un vecteur pour stocker les donnees
data = numpy.zeros((sampleNb,1))

# Recuperation des donnees
#serialPort.write(0)               # Envoi du caractere de synchro
for a in range(sampleNb):
    tmp1 = serialPort.read(1)       # on lit 1 octet sur le port serie
    tmp2 = unpack('B', tmp1)        # on le convertit en entier
    data[a] = tmp2[0] #3.3*tmp2[0]/256       # on calcule la tension correspondante, en V

# Deconnexion du dsPIC
serialPort.close()

# Affichage des donnees
endTime = sampleNb*samplePer                # Duree de l'acquisition, en s
time = numpy.linspace(0,endTime,sampleNb)   # Vecteur des instants d'echantillonnage
pyplot.plot(time, data)
pyplot.grid()
pyplot.xlabel('t (s)')
pyplot.ylabel('v (cm/s)')
pyplot.show()

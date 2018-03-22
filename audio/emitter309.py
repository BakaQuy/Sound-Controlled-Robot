# -*- coding:utf-8 -*-

from tkinter import *
from tkinter import ttk
from module309 import *


def sendBtEvFc():
    "Envoie la commande définie par 'commandSelectCb' et 'dataEn'"
    command = commandSelectCb.get()                         # récupèration de la commande
    dataStr = dataEn.get()                                  # récupération du paramètre (en string)
    if dataStr.isdigit():                                   # si le paramètre est un nombre
        data = int(dataStr)                                     # on le traduit
    else:                                                   # sinon
        data = 0                                                # on le choisit nul
    if command==commandList[0]:                             # Vérifie si le paramètre est dans
        if (data>127) or (data<-128):                       # l'intervalle autorisé pour la commande
            data = 0                                        # si non, on le choisit nul
    if command==commandList[1] or command==commandList[2]:  # Vérifie si le paramètre est dans
        if (data>127) or (data<0):                          # l'intervalle autorisé pour la commande
            data = 0                                        # si non, on le choisit nul
    if data!=0:                                             # si le paramètre n'est pas nul
        f = createFrame(command,data)                           # on construit la trame de bit
        s = createSignal(f)                                     # puis le signal audio
        playSignal(s)                                           # et on le joue
#        messageDisplay.config(test=f)
        print(f)
    else:                                                   # sinon
        dataEn.delete(0, END)                               # on efface le champ d'entrée
        dataEn.insert(0,'0')                                # pour y mettre '0'



def stopBtEvFc():
    print(commandList)
    f = createFrame(commandList[3],0)                       # on construit la trame de bit
    s = createSignal(f)                                     # puis le signal audio
    playSignal(s)                                           # et on le joue


# MAIN PROGRAM
###############
# Fenêtre principale
fen = Tk()                                                  # crée la fenêtre principale
fen.title('Emetteur FSK pour ELEC-H-309')                   # définit le titre de la fenêtre
# Choix de la commande
commandSelectCb = ttk.Combobox(fen)                         # crée la combo box pour la sélection de la commande
commandSelectCb['values'] = commandList[0:3]                # assigne la liste des commandes à la combo box
commandSelectCb.set(commandList[0])                         # définit la commande par défaut
commandSelectCb['state'] = 'readonly'                       # rend la liste non éditable
commandSelectCb.grid(row=0,column=0)                        # affiche le widget
# Entrée du paramètre
dataEn = ttk.Entry(fen)                                     # crée le champ d'entrée pour le paramètre de la commande
dataEn.insert(0,'10')                                       # initialise le paramètre
dataEn.grid(row=0,column=1)                                 # affiche le widget
# Bouton d'envoi
sendBt = ttk.Button(fen)                                    # crée le bouton d'envoi
sendBt.config(text="Go!")                                   # définit le texte du bouton
sendBt.config(command=sendBtEvFc)                           # définit la fonction associée au bouton
sendBt.grid(row=1,column=0)                                 # affiche le bouton
# Affichage de la trame
#messageDisplay = ttk.Label(fen)
#messageDisplay.pack()
# affiche la fenêtre (et bloque l'exécution du programme)
fen.mainloop()

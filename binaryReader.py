#!/usr/bin/python

# v.1 John Harrison 20-FEB-2015
# uses espeak. Could be changed to festival:
# echo "(SayText \"gimme binary\")" | festival --pipe

import subprocess, serial, time

# CONSTANTS
START_CARD_TOKEN = chr(30+ord('A')-1) # '^'
NO_CARD_TOKEN = chr(31+ord('A')-1) # '_'

# SETUP
word = ""
ser = serial.Serial('/dev/ttyUSB0', 9600,
                    bytesize=serial.EIGHTBITS,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    xonxoff=0,
                    rtscts=0)

# LOOP
while(1):
    x = ser.read(1)          # read one byte
    if x == START_CARD_TOKEN:
            subprocess.call(["toilet","-f", "mono12", "-F","gay","Got Card"])
            subprocess.call(["espeak","-ven+f4","Card Initialized"])        
            word = ""
    elif x == NO_CARD_TOKEN:
        if word != "":
            # print and say word
            subprocess.call(["toilet","-f", "mono12", "-F","gay",word])
            subprocess.call(["espeak","-ven+f4",word])
    else:
        # new letter received
        # print and say the letter
        subprocess.call(["toilet","-f", "mono12", "-F","gay",x])
        subprocess.call(["espeak","-ven+f4","-m","<speak><say-as interpret-as='characters'>"+str(x)+"</say-as></speak>"])
        word += x

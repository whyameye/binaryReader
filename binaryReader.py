#!/usr/bin/python

import subprocess

word = ""
ser = serial.Serial('/dev/ttyACM0', 115200)

while(1):
    letter = ser.read()          # read one byte
    if x == '_':
        # no card
        if word != "":
            # print and say word
            subprocess.call(["toilet","-f", "mono12", "-F","gay",word])
            subprocess.call(["espeak",word])
    else:
        # new letter received
        # print and say the letter
        subprocess.call(["toilet","-f", "mono12", "-F","gay",chr(letter)])
        subprocess.call(["espeak","<speak><say-as interpret-as='characters'>"+str(letter)+"</say-as></speak>"])
        word += chfr(letter)

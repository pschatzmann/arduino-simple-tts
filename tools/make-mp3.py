#!/usr/bin/env python3
#
# Generates mp3 audio files from the audio.csv input file.
# make sure that you install gtts: pip install gtts
#
import csv
import os
import gtts

directory = "out/audio"
ext = "mp3"
os.makedirs(directory,exist_ok=True)

with open('audio.csv') as csvfile:
     audioreader = csv.DictReader(csvfile, delimiter=',')
     for row in audioreader:
         name = row['Name']
         text = row['Text']
         tts = gtts.gTTS(text)
         filename = directory + "/" + name + "." + ext
         tts.save(filename)
         print(filename)

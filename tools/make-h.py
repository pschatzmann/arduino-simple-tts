#!/usr/bin/env python3
#
# Generates header files from the mp3 audio.
# make sure that you have xxd installed
#
import os
import sys
from pathlib import Path

directory = "out/audio"
h_directory = "out/h"
os.makedirs(h_directory,exist_ok=True)

for file in os.listdir(directory):
    filename = Path(file)
    filename_h = filename.with_suffix('.h')
    path_h = h_directory+"/"+str(filename_h)
    cmd = "xxd -i "+directory+"/"+file+" " + path_h
    print(cmd)
    os.system(cmd)

    # Read in the file
    with open(path_h, 'r') as file :
        filedata = file.read()

    # Replace the target string
    filedata = filedata.replace('unsigned char', '#pragma once\nconst unsigned char')
    # remove output path from variable names
    filedata = filedata.replace('out_audio_', '')

    # Write the file out again
    with open(path_h, 'w') as file:
        file.write(filedata)

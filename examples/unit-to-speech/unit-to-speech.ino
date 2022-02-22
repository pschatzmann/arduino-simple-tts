/**
 * @file unit-to-speech.ino
 * @author Phil Schatzmann
 * @brief Demo that translates numbers with units into speech
 * I used it with an AudioKit. If you want to use it with a regular micrcontroller
 * - remove the include "AudioLibs/AudioKit.h"
 * - replace the  AudioKitStream with I2SStream, AnalogAudioStream, PMWAudioStream
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "SimpleTTS.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "Desktop.h" // some special logic for desktop builds

#ifndef IS_DESKTOP
#include "AudioLibs/AudioKit.h" // for AudioKit
#endif

NumberUnitToText utt;
AudioKitStream out; // Replace with desired class e.g. I2SStream
MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(utt, out, mp3, dictionary);

double number = 1.1;

void setup(){
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Info);

    // setup out
    auto cfg = out.defaultConfig(); 
    cfg.sample_rate = 24000;
    cfg.channels = 1;
    out.begin(cfg);
}

void increment() {
    number +=1;
}

void loop() {
    // speach output
    utt.say(number, "usd");

    increment();
    delay(1000);
}
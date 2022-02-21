/**
 * @file number-to-speech.ino
 * @author Phil Schatzmann
 * @brief Demo that translates numbers into speech
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
#include "AudioLibs/AudioKit.h"
#endif

NumberToText ntt;
AudioKitStream out; // Replace with desired class e.g. I2SStream
MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(ntt, out, mp3, dictionary);

int64_t number = 1;

void setup(){
    Serial.begin(115200);
    // setup out
    auto cfg = out.defaultConfig(); 
    cfg.sample_rate = 24000;
    cfg.channels = 1;
    out.begin(cfg);
}

void increment() {
   // increment number by some value
  if(number<1000000000){
    // grow numbers fast  
    number *=10;
  } else {
    // grow numbers slow
    number +=1;
  }
}

void loop() {
    // speach output
    ntt.say(number);

    increment();
    delay(1000);
}
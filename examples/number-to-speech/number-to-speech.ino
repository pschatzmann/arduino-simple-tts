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
// #include "AudioLibs/AudioKit.h"

NumberToText ntt;
I2SStream i2s; // Replace with desired class e.g. AudioKitStream
MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(ntt, i2s, mp3, dictionary);

float number = -123.431;

void setup(){
    Serial.begin(115200);
    // setup i2s
    auto cfg = i2s.defaultConfig(); 
    cfg.sample_rate = 24000;
    cfg.channels = 1;
    i2s.begin(cfg);
}

void loop() {
    // speach output
    ntt.say(number);

    // increment number by some value
    number *=10;
    number++;
    delay(1000);
}
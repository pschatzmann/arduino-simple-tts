/**
 * @file unit-to-speech.ino
 * @author Phil Schatzmann
 * @brief Demo that translates numbers with units into speech with output as I2S
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "SimpleTTS.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "Desktop.h"

I2SStream i2s;
VolumeStream volume(i2s);
SilenceRemovalConverter<int16_t> rem(8, 2);
ConvertedStream<int16_t,SilenceRemovalConverter<int16_t>> out(volume, rem); 

MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
NumberUnitToText utt;
TextToSpeech tts(utt, out, mp3, dictionary);

double number = 1.1;

void setup(){
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Info);
    // setup out
    auto cfg = i2s.defaultConfig(); 
    cfg.sample_rate = 24000;
    cfg.channels = 1;
    i2s.begin(cfg);

    // define volume
    volume.setVolume(0.6);

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
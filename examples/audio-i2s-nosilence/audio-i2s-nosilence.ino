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
VolumeOutput volume(i2s);
SilenceRemovalStream<int16_t> out(volume);

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

    // remoeve the last 5 samples if amplidue is below 1
    out.begin(5, 1);
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
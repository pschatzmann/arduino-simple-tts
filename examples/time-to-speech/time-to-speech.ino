
/**
 * @file time-to-speech.ino
 * @author Phil Schatzmann
 * @brief Test sketch which announces the indicated time
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "AudioTools.h"
#include "SimpleTTS.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "Desktop.h" // some special logic for desktop builds
#ifndef IS_DESKTOP
#include "AudioLibs/AudioKit.h"
#endif


TimeToText ttt;
AudioKitStream out; // Replace with desired class e.g. I2SStream
MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(ttt, out, mp3, dictionary);
int hour=0, minute=0;

void setup(){
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Info);
 
    // setup out
    auto cfg = out.defaultConfig(); 
    cfg.sample_rate = 24000;
    cfg.channels = 1;
    out.begin(cfg);
}

void addMinutes(int minutes) {
    minute+=minutes;
    if (minute>=60){
        minute=0;
        hour++;;
    }
    if (hour>=24){
        hour = 0;
    }
}

void loop() {
    // speach output
    ttt.say(hour,minute);

    // generate next time
    delay(2000);
    addMinutes(10);
}

/**
 * @file time-to-speech.ino
 * @author Phil Schatzmann
 * @brief Test sketch which announces the indicated time. 
 * I tested the sketch with an AudioKit. If you want to use it with a regular micrcontroller
 * - remove the include "AudioLibs/AudioKit.h"
 * - replace the  AudioKitStream with I2SStream, AnalogAudioStream, PMWAudioStream
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "AudioTools.h"
#include "SimpleTTS.h"
#include "AudioDictionarySD.h"
#include "AudioCodecs/CodecMP3Helix.h"

#ifndef IS_DESKTOP
#include "AudioLibs/AudioKit.h" // for AudioKit
#endif

TimeToText ttt;
MP3DecoderHelix mp3;
const char* path = "/tts";
AudioDictionarySD dictionary(path, "mp3", PIN_AUDIO_KIT_SD_CARD_CS);
AudioKitStream out;  // Replace with desired output class e.g. I2SStream
TextToSpeech tts(ttt, out, mp3, dictionary);
int hour=0, minute=0;

void setup(){
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Warning);
 
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

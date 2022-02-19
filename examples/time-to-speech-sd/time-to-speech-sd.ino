
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
#include "AudioDictionarySD.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioLibs/AudioKit.h"

TimeToText ttt;
AudioKitStream i2s; // Replace with desired class e.g. AudioKitStream
MP3DecoderHelix mp3;
const char* path = "/tts";
AudioDictionarySD dictionary(path, "mp3", PIN_AUDIO_KIT_SD_CARD_CS);
TextToSpeech tts(ttt, i2s, mp3, dictionary);

void setup(){
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Info);
 
    // setup i2s
    auto cfg = i2s.defaultConfig(); 
    cfg.sample_rate = 24000;
    cfg.channels = 1;
    i2s.begin(cfg);

    // speach output
    ttt.say(11,40);
}

void loop() {

}
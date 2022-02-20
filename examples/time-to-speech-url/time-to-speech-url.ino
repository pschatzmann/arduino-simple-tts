
/**
 * @file time-to-speech.ino
 * @author Phil Schatzmann
 * @brief Test sketch which announces the indicated time - the audio files are loaded from the internet.
 * However I think this is rather too slow...
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "AudioTools.h"
#include "SimpleTTS.h"
#include "AudioDictionaryURL.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioLibs/AudioKit.h"


const char *ssid = "ssid";
const char *password = "password";
const char *url = "https://pschatzmann.github.io/arduino-simple-tts/audio/";

TimeToText ttt;
URLStream in(ssid, password); 
AudioDictionaryURL dictionary(in, url, "mp3");
MP3DecoderHelix mp3;
AudioKitStream out; // Replace with desired output class e.g. I2SStream
TextToSpeech tts(ttt, out, mp3, dictionary);
int hour=0, minute=0;

void setup(){
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Warning);
 
    // setup in
    auto cfg = out.defaultConfig(); 
    cfg.sd_active = false; // for AudioKitStream to use all pins
    cfg.sample_rate = 24000;
    cfg.channels = 1;
    out.begin(cfg);

    // speach output
    ttt.say(11,40);
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

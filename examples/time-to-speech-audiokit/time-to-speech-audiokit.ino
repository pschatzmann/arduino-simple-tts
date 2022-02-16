
#include "SimpleTTS.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioLibs/AudioKit.h"

TimeToText ttt;
AudioKitStream i2s;
MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(ttt, i2s, mp3, dictionary);
SimpleTime v_time;

void setup(){
    Serial.begin(115200);
    // setup i2s
    auto cfg = i2s.defaultConfig(); 
    cfg.sample_rate = 24000;
    i2s.begin(cfg);
}

void loop() {
    // speach output
    ttt.say(v_time);

    // increment time by 5 minutes
    v_time.minute+=5;
    if (v_time.minute>=60){
        v_time.hour++;
        v_time.minute = 0;
    }
    if (v_time.hour>=24){
        v_time.hour = 0;
    }
    delay(2000);
}

#include "SimpleTTS.h"
#include "AudioCodecs/CodecMP3Helix.h"

TimeToText ttt;
I2SStream i2s;
MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(ttt, i2s, mp3, dictionary);

void setup(){
    Serial.begin(115200);
    // setup i2s
    auto cfg = i2s.defaultConfig(); 
    cfg.sample_rate = 24000;
    i2s.begin(cfg);

    // speach output
    ttt.say(12,00);
}

void loop() {

}
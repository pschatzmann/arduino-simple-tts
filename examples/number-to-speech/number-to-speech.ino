
#include "SimpleTTS.h"
#include "AudioCodecs/CodecMP3Helix.h"

NumberToText ntt;
I2SStream i2s;
MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(ntt, i2s, mp3, dictionary);

float number = -123.431

void setup(){
    Serial.begin(115200);
    // setup i2s
    auto cfg = i2s.defaultConfig(); 
    i2s.begin(cfg);
}

void loop() {
    // speach output
    tts.say(number);

    // increment number by some value
    number *=10;
    number++;
    delay(1000);
}
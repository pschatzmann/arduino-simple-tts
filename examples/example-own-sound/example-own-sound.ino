#define NO_AUDIO_EXAMPLES
#include "SimpleTTS.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "speechArray.h"
//#include "Desktop.h" // some special logic for desktop builds

I2SStream i2s;
VolumeStream volume(i2s);
//SilenceRemovalConverter<int16_t> rem(8, 2);
//ConvertedStream<int16_t, SilenceRemovalConverter<int16_t>> out(volume, rem);
AudioDictionary dictionary(MyAudioDictionaryValues);

MP3DecoderHelix mp3;
TextToSpeechQueue tts(volume, mp3, dictionary);
int idx=0;

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup out
  auto cfg = i2s.defaultConfig();
  cfg.i2s_format = I2S_LSB_FORMAT;  //or I2S_STD_FORMAT
  cfg.sample_rate = 24000;
  cfg.channels = 1;
  i2s.begin(cfg);

  // setting the volume
  volume.setVolume(0.2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (tts.isEmpty()) {
    delay(1000); // in your real sketch: avoid delays
    tts.say(MyAudioDictionaryValues[idx++].name); // but better use a full sencense
    if (idx>58){
      idx = 0;      
    }
  }

  tts.process();
}

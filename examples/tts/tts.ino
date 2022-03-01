#define NO_AUDIO_EXAMPLES
#include "AudioCodecs/CodecMP3Helix.h"
#include "SimpleTTS.h"
#include "speechArray.h"

I2SStream i2s;
VolumeStream volume(i2s);
SilenceRemovalConverter<int16_t> rem(8, 2);
ConvertedStream<int16_t, SilenceRemovalConverter<int16_t>> out(volume, rem);
AudioDictionary dictionary(MyAudioDictionaryValues);

MP3DecoderHelix mp3;
TextToSpeechQueue tts(out, mp3, dictionary);

void setup() {
  // put your setup code here, to run once:
  // setup out
  auto cfg = i2s.defaultConfig(TX_MODE);
  cfg.i2s_format = I2S_LSB_FORMAT;
  cfg.sample_rate = 24000;
  cfg.channels = 1;
  i2s.begin(cfg);

  // setting the volume
  volume.setVolume(0.2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (tts.isEmpty()) {
    delay(1000);
    tts.say("ONE");
  }

  tts.process();
}

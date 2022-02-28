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

I2SStream out;
VolumeStream volume(out);

MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
NumberUnitToText utt;
TextToSpeechQueue queue(utt, volume, mp3, dictionary);

double number = 1.1;

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setting the volume
  volume.setVolume(0.6);

  // setup out
  auto cfg = out.defaultConfig();
  cfg.sample_rate = 24000;
  cfg.channels = 1;
  out.begin(cfg);
}

void loop() {
  // speach output when queue is empty
  if (queue.size()==0) {
    delay(1000);
    utt.say(number, "usd");
    number++;
  }

  Serial.print("next word:");
  Serial.println(queue[0]);

  // process next word in queue
  queue.process();
}
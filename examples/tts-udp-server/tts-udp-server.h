/**
 * @file tts-udp-server.h
 * @author Phil Schatzmann
 * @brief A simple UDP server which speaks out words
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "SimpleTTS.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioLibs/Communication.h"

I2SStream out;
VolumeStream volume(out);

MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
NumberToText ntt;
TextToSpeechQueue queue(ntt, volume, mp3, dictionary);
UDPStream udp;
char buffer[160];

// Provides a fixed string from dictionary for a temporary input string
const char *toName(char *str) {
  for (auto e : ExampleAudioDictionaryValues) {
    Str str1(str);
    str1.trim();
    if (str1.equalsIgnoreCase(e.name)) {
      return e.name;
    }
  }
  return nullptr;
}

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
  // fill queue with words from UDP
  while (udp.available()) {
    if (udp.readBytesUntil(' ', (uint8_t*)buffer, 160) > 0) {
      Str bufferStr((const char*)buffer);
      bufferStr.trim();
      if (bufferStr.isNumber()) {
        double value = bufferStr.toDouble();
        int decimals = bufferStr.numberOfDecimals();
        for (auto str : ntt.say(value, decimals)) {
          queue.say(toName((char*)str));
        }
      } else {
        queue.say(toName(buffer));
      }
    }
  }
  // play next word in queue
  queue.process();
}
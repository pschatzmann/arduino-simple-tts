/**
 * @file talking-time.ino
 * @author Phil Schatzmann
 * @brief A talking clock: You can define how ofter the time is announced with setEveryMinutes
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "SimpleTTS.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "time/TimeInfoESP32.h" // or alternative TimeInfoRTC.h
#include "AudioTools/AudioLibs/AudioBoardStream.h"

// Output
TimeToText ttt;
AudioBoardStream out(AudioKitEs8388V1); // replace with alterntive Audio Sink if needed: AnalogAudioStream, I2SStream etc.
MP3DecoderHelix mp3;
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(ttt, out, mp3, dictionary);

// Determine Time
TimeInfo timeInfo;
const char* ssid = "SSID";
const char* password = "password";

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup i2s
  auto cfg = out.defaultConfig();
  cfg.sample_rate = 24000;
  cfg.channels = 1;
  out.begin(cfg);

  // We announce the time only every 5 minutes
  timeInfo.setEveryMinutes(5);

  // start WIFI and time (ESP32)
  timeInfo.begin(ssid, password);

  //uRTCLib rtc(0x68);  // alternarive using RTC
  //timeInfo.begin(rtc);

  // time at startup
  ttt.say(timeInfo.time());
}


void loop() {
  // speach output
  if (timeInfo.update()){
    ttt.say(timeInfo.time());
    tts.say("SILENCE");  // ESP32: prevent noise at end
  }
}
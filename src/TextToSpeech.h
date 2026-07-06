#pragma once
#ifndef NO_SPEECH
#include "AudioTools/CoreAudio/AudioOutput.h"
#include "AudioTools/CoreAudio/AudioStreams.h"
#include "AudioTools/CoreAudio/AudioTypes.h"
#include "NumberToText.h"
#include "AudioDictionary.h"

namespace simple_tts {

/**
 * @brief Audio output from text via the indicated audio sink. The text
 * components need to be prerecorded and available as audio files.
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class TextToSpeech {
 public:
  /// TextToSpeech which support a SimpleTTSBase 
  TextToSpeech(SimpleTTSBase &tts, AudioStream &sink, AudioDecoder &decoder,
               AudioDictionaryBase &dict) {
    tts.registerCallback(callback, this);
    p_tts = &tts;
    p_dictionary = &dict;
    p_decoder = &decoder;
    p_sink = &sink;
    decodedStream = new audio_tools::EncodedAudioStream(&sink, &decoder);
    begin();
  }

  TextToSpeech(SimpleTTSBase &tts, AudioOutput &sink, AudioDecoder &decoder,
               AudioDictionaryBase &dict) {
    tts.registerCallback(callback, this);
    p_tts = &tts;
    p_dictionary = &dict;
    p_decoder = &decoder;
    p_sink = &sink;
    decodedStream = new audio_tools::EncodedAudioStream(&sink, &decoder);
    begin();
  }

  /// Default Constructor
  TextToSpeech(AudioStream &sink, AudioDecoder &decoder,
               AudioDictionaryBase &dict) {
    p_dictionary = &dict;
    p_decoder = &decoder;
    p_sink = &sink;
    decodedStream = new audio_tools::EncodedAudioStream(&sink, &decoder);
    begin();
  }

  /// Default Constructor for AudioOutput
  TextToSpeech(AudioOutput &sink, AudioDecoder &decoder,
               AudioDictionaryBase &dict) {
    p_dictionary = &dict;
    p_decoder = &decoder;
    p_sink = &sink;
    decodedStream = new audio_tools::EncodedAudioStream(&sink, &decoder);
    begin();
  }

  /// Constructor w/o output: you need to call begin() and setOutput()
  TextToSpeech(AudioDecoder &decoder,
               AudioDictionaryBase &dict) {
    p_dictionary = &dict;
    p_decoder = &decoder;
  }


  ~TextToSpeech() { delete decodedStream; }

  /// a simple API to say one of the supported words
  bool say(const char *word) {
    if (decodedStream == nullptr) {
      LOGE("No output stream defined. Use setOutput() to define the output stream.");
      return false;
    }
    if (word == nullptr) return false;
    LOGI("say: %s",word);
    AudioStream *mp3Stream = p_dictionary->get(word);
    if (mp3Stream != nullptr) {
      if (!active) begin();
      mp3Stream->begin();
      copier.begin(*decodedStream, *mp3Stream);
      copier.copyAll(0, 0);
      copier.end();
      mp3Stream->end();
      // if we use free rtos, we give other processes the chance to 
      delay(1);
      return true;
    } else {
      LOGE("Word not available in dictionary: %s", word);
      return false;
    }
  }

  bool begin() {
    active = p_decoder->begin();
    return active;
  }

  void end() {
    p_decoder->end();
    active = false;
  }

  /// a simple API to say multiple of the supported words
  bool say(audio_tools::Vector<const char *> words) {
    bool result = true;
    if (!begin()) return false;
    for (auto word : words) {      
      if (!say(word)) result = false;
    }
    end();
    return result;
  }

  /// writes silence for the indicated ms
  void delay(uint32_t delay_ms){
      uint8_t buffer[1024] = {0};
      unsigned long timeout = millis()+delay_ms;
      while(timeout>millis()){
         p_sink->write((const uint8_t*)buffer,1024);
      }
  }

  void setOutput(AudioStream &sink) {
    p_sink = &sink;
    if (decodedStream) delete decodedStream;
    decodedStream = new audio_tools::EncodedAudioStream(&sink, p_decoder);
  }

  void setOutput(AudioOutput &sink) {
    p_sink = &sink;
    if (decodedStream) delete decodedStream;
    decodedStream = new audio_tools::EncodedAudioStream(&sink, p_decoder);
  }

 protected:
  bool active = false;
  NumberToText ntt;
  audio_tools::AudioDecoder *p_decoder = nullptr;
  audio_tools::EncodedAudioStream *decodedStream = nullptr;  // Decoding stream
  SimpleTTSBase *p_tts = nullptr;               // Text source
  AudioDictionaryBase *p_dictionary = nullptr;  // Dictionary to access audio data
  audio_tools::StreamCopy copier;               // copy in to out
  Print *p_sink=nullptr;

  /// callback which says the words
  static void callback(audio_tools::Vector<const char *> words, void* ref) { 
    TextToSpeech* self = (TextToSpeech*)ref;
    self->say(words); }
};

}  // namespace simple_tts

#endif
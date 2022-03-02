#pragma once
#ifndef NO_SPEECH
#include <list>

#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioDictionary.h"
#include "AudioTools/AudioOutput.h"
#include "AudioTools/AudioStreams.h"
#include "AudioTools/AudioTypes.h"
#include "NumberToText.h"

namespace simple_tts {

/**
 * @brief Audio output from text via the indicated audio sink. The text
 * components need to be prerecorded and available as audio files.
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class TextToSpeechQueue {
 public:
  /// TextToSpeech which support a SimpleTTSBase
  TextToSpeechQueue(SimpleTTSBase &tts, AudioStream &sink,
                    AudioDecoder &decoder, AudioDictionaryBase &dict) {
    tts.registerCallback(callback, this);
    p_tts = &tts;
    p_dictionary = &dict;
    p_decoder = &decoder;
    p_sink = &sink;
    decodedStream = new audio_tools::EncodedAudioStream(&sink, &decoder);
    begin();
  }

  TextToSpeechQueue(SimpleTTSBase &tts, AudioPrint &sink, AudioDecoder &decoder,
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
  TextToSpeechQueue(AudioStream &sink, AudioDecoder &decoder,
                    AudioDictionaryBase &dict) {
    p_dictionary = &dict;
    p_decoder = &decoder;
    p_sink = &sink;
    decodedStream = new audio_tools::EncodedAudioStream(&sink, &decoder);
    begin();
  }

  ~TextToSpeechQueue() { delete decodedStream; }

  /// a simple API to add a single c string to the queue
  void say(const char *word) {
    LOGI("%s", word);
    queue.push_back(word);
  }

  /// Adds a word to the front
  void sayNow(const char *word) { queue.push_front(word); }

  /// Addds an array of c strings
  void say(const char *word[], int size) {
    for (int j = 0; j < size; j++) {
      say(word[j]);
    }
  }

  /// Addds an array of c strings.
  template <size_t N>
  void say(char *(&word)[N]) {
    for (int j = 0; j < N; j++) {
      say(word[j]);
    }
  }

  /// a simple API to say multiple of the supported words
  void say(audio_tools::Vector<const char *> words) {
    for (auto word : words) {
      say(word);
    }
  }

  // Add this in the Arduino Loop -> we process the next word
  void process() {
    int size = queue.size();
    if (size > 0) {
      const char *word = (*this)[0];
      queue.pop_front();

      LOGI("say: %s  (size: %d -> %d)", word, size, queue.size());
      processWord(word);
      if (queue.size() == 0) {
        // end();
        silence(1);
      }
    }
  }

  /// Output of word to audio sink
  void processWord(const char *word) {
    AudioStream *mp3Stream = p_dictionary->get(word);
    if (mp3Stream != nullptr) {
      if (!active) {
        begin();
      }
      // decode to audio
      mp3Stream->begin();
      copier.begin(*decodedStream, *mp3Stream);
      copier.copyAll();
      copier.end();
      mp3Stream->end();

    } else {
      LOGE("Word not available in dictionary: %s", word);
    }
  }

  /// Sends silence to mp3 decoder for n secods
  void silence(int n=1){
    for (int j=0;j<n;j++){
      processWord("SILENCE");
    }
  }

  /// Returns the number of words to be spoken in the queue
  size_t size() { return queue.size(); }

  /// Returns true if the queue is empty
  bool isEmpty() { return size() == 0; }

  /// Determines the nth word in the queue
  const char *operator[](int n) {
    if (n < size()) {
      // auto it = queue.begin();
      // std::advance(it, n);
      // return *it;
      return queue[n];
    }
    return nullptr;
  }

  /// Opens the processing
  void begin() {
    p_decoder->begin();
    active = true;
  }

  /// Ends the processing and clears the queue
  void end() {
    p_decoder->end();
    active = false;
    clear();
  }

  /// Clears the queue
  void clear() { queue.clear(); }

  /// writes silence for the indicated ms
  void delay(uint32_t delay_ms = 1000) {
    uint8_t buffer[1024] = {0};
    unsigned long timeout = millis() + delay_ms;
    while (timeout > millis()) {
      p_sink->write((const uint8_t *)buffer, 1024);
    }
  }

 protected:
  // std::list<const char *> queue;
  Vector<const char *> queue;
  bool active = false;
  NumberToText ntt;
  audio_tools::AudioDecoder *p_decoder = nullptr;
  audio_tools::EncodedAudioStream *decodedStream = nullptr;  // Decoding stream
  SimpleTTSBase *p_tts = nullptr;                            // Text source
  AudioDictionaryBase *p_dictionary =
      nullptr;                     // Dictionary to access audio data
  audio_tools::StreamCopy copier;  // copy in to out
  Print *p_sink = nullptr;


  /// callback which adds the words to the queue
  static void callback(audio_tools::Vector<const char *> words, void *ref) {
    TextToSpeechQueue *self = (TextToSpeechQueue *)ref;
    self->say(words);
  }
};

}  // namespace simple_tts

#endif
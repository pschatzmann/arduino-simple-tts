#pragma once
#ifndef NO_SPEECH
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioOutput.h"
#include "AudioTools/AudioStreams.h"
#include "AudioTools/AudioTypes.h"
#include "NumberToText.h"
#include "AudioDictionary.h"
#include "en/all.h"

namespace simple_tts {

// the audio includes do not have a namespace so we import them here

class TextToSpeech;

/**
 * @brief Audio output from text via the indicated audio sink. The text
 * components need to be prerecorded and available as audio files.
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class TextToSpeech {
 public:
  TextToSpeech(SimpleTTSBase &tts, AudioStream &sink, AudioDecoder &decoder,
               AudioDictionaryBase &dict) {
    tts.registerCallback(callback, this);
    p_tts = &tts;
    p_dictionary = &dict;
    p_decoder = &decoder;
    decodedStream = new audio_tools::EncodedAudioStream(&sink, &decoder);
  }

  ~TextToSpeech() { delete decodedStream; }

  /// a simple API to say one of the supported words
  void say(const char *word) {
    LOGI("say: %s",word);
    AudioStream *mp3Stream = p_dictionary->get(word);
    if (mp3Stream != nullptr) {
      mp3Stream->begin();
      copier.begin(*decodedStream, *mp3Stream);
      copier.copyAll();
      copier.end();
      mp3Stream->end();
    } else {
      LOGE("Word not available in dictionary: %s", word);
    }
  }

  /// a simple API to say multiple of the supported words
  void say(audio_tools::Vector<const char *> words) {
    p_decoder->begin();
    for (auto word : words) {      
      say(word);
    }
    p_decoder->end();
  }

 protected:
  NumberToText ntt;
  audio_tools::AudioDecoder *p_decoder = nullptr;
  audio_tools::EncodedAudioStream *decodedStream = nullptr;  // Decoding stream
  SimpleTTSBase *p_tts = nullptr;             // Text source
  AudioDictionaryBase *p_dictionary = nullptr;  // Dictionary to access audio data
  audio_tools::StreamCopy copier;                          // copy in to out

  /// callback which says the words
  static void callback(audio_tools::Vector<const char *> words, void* ref) { 
    TextToSpeech* self = (TextToSpeech*)ref;
    self->say(words); }
};

}  // namespace simple_tts

#endif
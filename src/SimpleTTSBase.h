#pragma once
#include <stdio.h>
#include <string.h>

#include "AudioTools.h"

namespace simple_tts {

/**
 * @brief Common Functionality for TTS classes
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class SimpleTTSBase {
 public:
  virtual audio_tools::Vector<const char*> & allTexts() = 0;
  virtual void registerCallback(void (*ptr)(audio_tools::Vector <const char*> words, void* refx), void*ref) {
    callback = ptr;
    reference = ref;
  }

 protected:
  void (*callback)(audio_tools::Vector < const char*> word, void* ref) = nullptr;
  void* reference = nullptr;
};

/**
 * @brief Dictionary which provides a Stream of Audio for the indicated word
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class AudioDictionaryBase {
 public:
  virtual AudioStream* get(const char* word) = 0;
};

/**
 * @brief An Entry for the in Memory Audio Dictionary
 * @author Phil Schatzmann
 * @copyright GPLv3
 *
 */
struct AudioDictionaryEntry {
  const char* name;
  AudioStream* audio;
};

/**
 * @brief An Entry for the in Memory Audio Dictionary which contains the full text
 * @author Phil Schatzmann
 * @copyright GPLv3
 * 
 */
struct AudioDictionaryEntryExt : AudioDictionaryEntry {
  const char* text = nullptr; // optional - when different from name
};

/**
 * @brief Just a simple structure with the hour and minutes
 *
 */
struct Time {
  int hour=0; 
  int minute=0;
  Time() = default;
  Time(int hour, int minutes){
    this->hour = hour;
    this->minute = minutes;
  }
};

}  // namespace simple_tts
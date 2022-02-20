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
  virtual audio_tools::Vector<const char*>& allTexts() = 0;
  virtual void registerCallback(
      void (*ptr)(audio_tools::Vector<const char*> words, void* refx),
      void* ref) {
    callback = ptr;
    reference = ref;
  }

  // Creates all missing audio recording files for the indicated source. We make sure
  // that the output is lowercase
  void printCSV(Print &out) {
    for (auto txt : allTexts()) {
      if (txt!=nullptr && strlen(txt)>0){
        StrExt str = txt; 
        str.toLowerCase(); // convert txt to lowercase
        out.print(str.c_str());
        out.print(", ");
        out.println(str.c_str());
      }
    }
  }

 protected:
  void (*callback)(audio_tools::Vector<const char*> word, void* ref) = nullptr;
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
 * @brief An Entry for the in Memory Audio Dictionary which contains the full
 * text
 * @author Phil Schatzmann
 * @copyright GPLv3
 *
 */
struct AudioSDEntry {
  const char* name;
  const char* text = nullptr;  // optional - when different from name
};

/**
 * @brief Just a simple structure with the hour and minutes
 *
 */
struct SimpleTime {
  int hour = 0;
  int minute = 0;
  SimpleTime() = default;
  SimpleTime(int hour, int minutes) {
    this->hour = hour;
    this->minute = minutes;
  }
  bool operator==(const SimpleTime& alt) {
    return alt.hour == hour && alt.minute == minute;
  }
  bool operator!=(const SimpleTime& alt) {
    return alt.hour != hour || alt.minute != minute;
  }
};

}  // namespace simple_tts
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
        out.print(",");
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
 * @author Phil Schatzmann
 * @copyright GPLv3
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

/**
 * @brief Convert numbers to string and provide integer and decimals
 * @author Phil Schatzmann
 * @copyright GPLv3
 *
 */

class Number {
public:
  void set(double value, int digits=2) {
    char format[10];
    // e.g. %0.2f for 2 digits.
    sprintf(format,"%%0.%df", digits);
    LOGD("format: %s", format);
    memset(buffer, 0, buffer_len);
    // convert to string
    sprintf(buffer, format, value);
    LOGD("number: %s",buffer);
    dot = strchr(buffer, '.');
    // split string
    *dot = 0;

    LOGD("int: %s", intValue());
    LOGD("dec: %s", decValues());
  }

  void set(int64_t wholeNumber) {
    memset(buffer,0,buffer_len);
    // convert to string
    sprintf(buffer, "%lld", wholeNumber);
    //ltoa(wholeNumber, buffer, 10);
    dot = buffer+strlen(buffer);
  }

  /// provides the full number
  const char* intValue() {
    return buffer;
  }

  /// provides the decimals after the .
  const char* decValues() {
    return dot+1;
  }

  // converts a decimal to a full number: 1 gives 10, 01 dives 1 if digits are 2. This is used e.g. for USD 1.1 which
  // will need to render 1 dollar and 10 cents; attn: 1.101 should still give 10 cents!
  const char* decAsInt(const char* decimals, int digits){
    memset(buffer, 0, buffer_len);
    memset(buffer,'0', digits);
    int len = min((int)strlen(decimals),digits);
    memcpy(buffer, decimals, len);
    return buffer;
  }

protected:
    // convert to string
    static const int buffer_len = 100;
    char buffer[buffer_len];
    char* dot=buffer;

};

}  // namespace simple_tts
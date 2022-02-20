#pragma once
#include "NumberToText.h"

namespace simple_tts {

/**
 * @brief Translates a time in hour and minutes into englich words. This
 * implementation builds upon the NumberToText class to handle the basic numbers
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class TimeToText : public SimpleTTSBase {
 public:
  /// converts a number to it's text representation
  audio_tools::Vector<const char *> &say(int hour, int minutes) {
    SimpleTime time(hour,minutes);
    return say(time);
  }

  /// converts a number to it's text representation
  audio_tools::Vector<const char *> &say(SimpleTime time) {
    result.clear();
    LOGI("say: %d:%d",time.hour, time.minute);
    if (time.minute < 0 || time.minute >= 60) {
      LOGE("Invalid minute (range 0-59): %d", time.minute);
      return result;
    }
    if (time.hour < 0 || time.hour > 24) {
      LOGE("Invalid hour (range 0-24): %d", time.hour);
      return result;
    }

    process(time);

    // provide result to callback
    if (callback){
        callback(result, reference);
    }

    return result;
  }

  /// provides all texts
  audio_tools::Vector<const char *> &allTexts() {
    result.clear();
    for (int j = 0; j < 10; j++) {
      add(j);
    }
    return result;
  }

 protected:
  audio_tools::Vector<const char *> result;
  NumberToText ntt;
  const int ITS = 0;
  const int NOON = 1;
  const int MIDNIGHT = 2;
  const int OCLOCK = 3;
  const int TO = 4;
  const int PAST = 5;
  const int AM = 6;
  const int PM = 7;
  const int HALF = 8;
  const int QUARTER = 9;

  const char *words[10] = {"ITS", "NOON", "MIDNIGHT", "OCLOCK", "TO",
                           "PAST", "AM",   "PM",       "HALF",    "QUARTER"};

  void add(int idx) { add(words[idx]); }

  void add(const char *str) { result.push_back(str); }

  void addAll(audio_tools::Vector<const char *> &words) {
    for (auto word : words) {
      add(word);
    }
  }

  void process(SimpleTime time) {
    add(ITS);
    // process exceptional times
    if (time.hour == 12 && time.minute == 00) {
      add(NOON);
      return;
    }
    if (time.hour == 0 && time.minute == 00) {
      add(MIDNIGHT);
      return;
    }

    // add minutes
    processMinutes(time);

    // add hour
    if (time.hour <= 12) {
      processHourAM(time);
    } else {
      processHourPM(time);
    }
    return;
  }

  void processMinutes(SimpleTime &time) {
    // process minutes
    if (time.minute == 0) {
      // do nothing
    } else if (time.minute == 15) {
      add(QUARTER);
      add(PAST);
    } else if (time.minute < 30) {
      addAll(ntt.say(time.minute,0u));
      add(PAST);
    } else if (time.minute == 30) {
      add(HALF);
      add(PAST);
    } else if (time.minute == 45) {
      time.hour += 1;
      add(QUARTER);
      add(TO);
    } else if (time.minute > 30) {
      time.hour += 1;
      time.minute = 60 - time.minute;
      addAll(ntt.say(time.minute,0u));
      add(TO);
    }
  }

  void processHourPM(SimpleTime time) {
    int hour = time.hour;
    hour -= 12;
    // we do not say 0pm -> 12pm
    if (hour == 0) {
      hour = 12;
    }
    addAll(ntt.say(hour,0u));

    if (time.minute==0){
      add(OCLOCK);
    }

    add(PM);
  }

  void processHourAM(SimpleTime time) {
    int hour = time.hour;
    // we do not say 0am -> 12am
    if (hour == 0) {
      hour = 12;
    }
    addAll(ntt.say(hour,0u));

    if (time.minute==0){
      add(OCLOCK);
    }

    add(AM);
  }
};

}  // namespace simple_tts

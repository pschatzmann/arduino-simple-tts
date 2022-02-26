#pragma once

#include "SimpleTTS.h"
#include "uRTCLib.h"

/**
 * @brief A simple class which provides the time with the help of a RTC library
 * https://github.com/Naguissa/uRTCLib
 */
class TimeInfo {
 public:
  void begin(uRTCLib &rtc) {
    p_rtc = &rtc;
    Wire.begin();
  }

  /// annonce only evry n minutes
  void setEveryMinutes(int n) { this->minutes = n; }

  // update only every 1 sec
  bool update() {
    bool result = false;
    if (millis() > timeout) {
      struct tm timeinfo;
	    p_rtc->refresh();
      // update time
      currentTime.hour = p_rtc->hour();
      currentTime.minute = p_rtc->minute();

      // check if we need to announce the time
      if (isDirty() && isRelevant()) {
        result = true;
        timeAnnounced = currentTime;
      }

      // update timeout
      timeout = millis() + 1000;
    }
    return result;
  }

  /// gets the actual time
  SimpleTime time() { return currentTime; }

 protected:
  int minutes = 5;
  unsigned long timeout;
  const long gmtOffset_sec = 3600;
  const int daylightOffset_sec = 3600;
  SimpleTime currentTime;
  SimpleTime timeAnnounced;
  uRTCLib *p_rtc = nullptr;

  // returns true if we need to announce a new time
  bool isDirty() { return currentTime != timeAnnounced; }
  // returns true if the minutes are a multiple
  bool isRelevant() { return currentTime.minute % minutes == 0; }
};
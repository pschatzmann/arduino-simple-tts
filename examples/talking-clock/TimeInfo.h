#pragma once
#include <WiFi.h>

#include "SimpleTTS.h"
#include "time.h"

/**
 * @brief A simple class which provides the time
 * This is working only on a ESP32 using a time server. If you have a processor
 * that does not support wifi you will need to replace this logic with one based
 * on a RTC module.
 */
class TimeInfo {
 public:
  void begin(const char* ssid, const char* password) {
    // connect to WiFi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println(" CONNECTED");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // setup initial time
    update();

    // disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  }

  /// annonce only evry n minutes
  void setEveryMinutes(int n) { this->minutes = n; }

  // update only every 1 sec
  bool update() {
    bool result = false;
    if (millis() > timeout) {
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return false;
      }
      Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      // update time
      currentTime.hour = timeinfo.tm_hour;
      currentTime.minute = timeinfo.tm_min;

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
  const char* ntpServer = "europe.pool.ntp.org";
  // For UTC +1.00 : 1 * 60 * 60 : 3600
  const long gmtOffset_sec = 3600;
  const int daylightOffset_sec = 3600;
  SimpleTime currentTime;
  SimpleTime timeAnnounced;

  // returns true if we need to announce a new time
  bool isDirty() { return currentTime != timeAnnounced; }
  // returns true if the minutes are a multiple
  bool isRelevant() { return currentTime.minute % minutes == 0; }
};
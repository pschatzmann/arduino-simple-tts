
#pragma once

#include <WiFi.h>

#include "SimpleTTSBase.h"

// by default we use the SdFat Library
#ifdef USE_SD
#include <SD.h>
typedef File AudioFile;
#else
#include <SdFat.h>
typedef File32 AudioFile;
typedef SdFat32 AudioFat;
#ifndef SDFAT_SPEED
#define SDFAT_SPEED 2
#endif
#endif

namespace simple_tts {

/**
 * @brief A simple Wrapper that let's a file pretend to be a AudioStream to
 * support the begin and end methods
 *
 */
template <class T>
class AudioStreamFileWrapper : public AudioStreamX {
 public:
  AudioStreamFileWrapper() = default;
  virtual bool begin(T &file) {
    p_file = &file;
    return true;
  }
  virtual bool begin() override {
    LOGI(LOG_METHOD);
    p_file->seek(0);
    return true;
  }
  virtual void end() override {
    LOGI(LOG_METHOD);
    p_file->close();
  }

  virtual size_t readBytes(uint8_t *buffer, size_t length) override {
    return p_file->readBytes((char *)buffer, length);
  }

  virtual size_t write(const uint8_t *buffer, size_t length) override {
    return p_file->write(buffer, length);
  }

  virtual int available() override { return p_file->available(); }

  virtual int availableForWrite() override {
    return p_file->availableForWrite();
  }

  operator bool() { return *p_file; }

 protected:
  T *p_file = nullptr;
};

/**
 * @brief A dictionary which is based on files stored on an SD card. By default
 * we use the SdFat library. You can use the SD library with #define USE_SD
 * before including this file
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class AudioDictionarySD : public AudioDictionaryBase {
 public:
  AudioDictionarySD(const char *path, const char *ext, int cs_pin = PIN_CS) {
    this->path = path;
    this->ext = ext;
    this->cs_pin = cs_pin;
  }

  ~AudioDictionarySD() { file.close(); }

  /// retrieves recorded audio file for the word
  AudioStream *get(const char *word) {
    setup();
    // provide new file
    const char *file_name = getFileWithPath(word);
    if (SD.exists(file_name)) {
      file = SD.open(file_name, FILE_READ);
      fileWrapper.begin(file);
      return &fileWrapper;
    }
    LOGE("File does not exist: %s", file_name);

    return nullptr;
  }

 protected:
  audio_tools::StrExt url_with_text;
  AudioFile file;
  AudioStreamFileWrapper<AudioFile> fileWrapper;
#ifndef USE_SD
  AudioFat SD;
#endif
  const char *path;
  const char *ext;
  StrExt file_path{40};  // allocate 40 bytes as a typical good initial size
  bool is_setup = false;
  int cs_pin = -1;

  void setup() {
    if (!is_setup) {
#ifdef USE_SD
      LOGI("Setup SD library");
      if (!SD.begin(cs_pin)) {
        LOGE("SD.begin failed for cs_pin: %d", cs_pin);
        return;
      }
#else
      LOGI("Setup SdFat library");
      if (!SD.begin(
              SdSpiConfig(cs_pin, DEDICATED_SPI, SD_SCK_MHZ(SDFAT_SPEED)))) {
        LOGE("SD.begin failed for cs_pin: %d", cs_pin);
        return;
      }
#endif
      if (!SD.exists(path)) {
        LOGI("Creating directory: %s", path)
        if (!SD.mkdir(path)) {
          LOGE("Could not create directory: %s", path);
        }
      }
      is_setup = true;
    }
  }

  // determines the filename for the word
  const char *getFileWithPath(const char *name) {
    file_path = path;
    file_path.add("/");
    file_path.add(name);
    file_path.add(".");
    file_path.add(ext);
    file_path.toLowerCase();
    const char *str = file_path.c_str();
    LOGI("%s -> %s", name, str);
    return str;
  }
};

}  // namespace simple_tts

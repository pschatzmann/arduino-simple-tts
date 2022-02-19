
#pragma once

#include <WiFi.h>
#include <SD.h>
//#include <SPI.h>

#include "SimpleTTSBase.h"

namespace simple_tts {

#include "en/all.h"


/**
 * @brief A dictionary which is based on files stored on an SD card
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class Importer {
 public:
  Importer(const char *path, const char *ext, int cs_pin = -1) {
    this->path = path;
    this->ext = ext;
    this->cs_pin = cs_pin;
  }

  ~Importer() { file.close(); }


  // Creates all missing audio recording files for the indicated source
  void initialLoad(SimpleTTSBase &source, const char *url, const char *mime) {
    auto texts = source.allTexts();
    for (auto txt : texts) {
      initialLoad(txt, txt, url, mime);
    }
  }

  // Loads the audio for the indicated entries. The last entry must be
  // terminated with nullptr
  void initialLoad(AudioSDEntry *entries, const char *url, const char *mime) {
    AudioSDEntry *e = entries;
    while (e != nullptr && e->name != nullptr) {
      initialLoad(e->name, e->text, url, mime);
      e++;
    }
  }

  // Creates a single audio files for the indicated text
  void initialLoad(const char *name, const char *text, const char *url,
                   const char *mime) {
    setup();
    url_with_text = url;
    url_with_text.replace("@", text);
    LOGI("url: %s", url_with_text.c_str());

    const char *file = getFileWithPath(name);
    LOGI("file: %s", file);
    if (SD.exists(file)) {
      SD.remove(file);
    }
    File newFile = SD.open(file, FILE_WRITE);
    bool ok = copy(newFile, url_with_text.c_str(), mime);
    if (!ok){
      LOGE("No data available: file '%s' will be deleted", file);
      SD.remove(file);
    }
  }

 protected:
  audio_tools::StrExt url_with_text;
  File file;
  AudioStreamFileWrapper fileWrapper;
  const char *path;
  const char *ext;
  char file_path[200];
  bool is_setup = false;
  int cs_pin = -1;
  URLStream url_stream;
  StreamCopy cp;

  void setup() {
    if (!is_setup) {
      LOGI("setup SD..");
      if (cs_pin != -1) {
        SD.begin(cs_pin);
      } else {
        SD.begin();
      }
      if (!SD.exists(path)) {
        LOGI("Creating directory: %s", path)
        SD.mkdir(path);
      }
      is_setup = true;
    }
  }

  // determines the filename for the word
  const char *getFileWithPath(const char *name) {
    file_path[0] = 0;
    strcat(file_path, path);
    strcat(file_path, "/");
    strcat(file_path, name);
    strcat(file_path, ".");
    strcat(file_path, ext);
    LOGI("%s -> %s", name, file_path);
    return (const char *)file_path;
  }

  bool copy(File &file, const char *url, const char *mime) {
    url_stream.begin(url, mime);
    cp.begin(file, url_stream);
    bool result = cp.copyAll(0);
    url_stream.end();
    LOGI("file size: %d Kbyte", (int) file.size() / 1024);
    file.close();
    return result;
  }
};

}  // namespace simple_tts

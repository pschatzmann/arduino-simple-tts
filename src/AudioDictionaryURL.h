#pragma once

#include "AudioHttp/URLStream.h"
#include "SimpleTTSBase.h"

namespace simple_tts {

/**
 * @brief A dictionary which is based on audio files that can be accessed via
 * URLs
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class AudioDictionaryURL : public AudioDictionaryBase {
public:
  AudioDictionaryURL(URLStream &urlStream, const char *url, const char *ext) {
    p_url = &urlStream;
    url_cstr = url;
    ext_cstr = ext;
  }

  AudioStream *get(const char *word) {
    url_str = url_cstr;
    if (!url_str.endsWith("/")){
      url_str.add("/");
    }
    url_str.add(word);
    url_str.add(".");
    url_str.add(ext_cstr);
    url_str.toLowerCase();
    LOGI("Using url: %s", url_str.c_str());
    p_url->begin(url_str.c_str());
    return p_url;
  }

protected:
  const char *url_cstr;
  const char *ext_cstr;
  StrExt url_str;
  URLStream *p_url;
};

} // namespace simple_tts
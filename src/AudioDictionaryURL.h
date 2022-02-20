#pragma once

#include "SimpleTTSBase.h"

namespace simple_tts {

#include "en/all.h"

/**
 * @brief A dictionary which is based on audio files that can be accessed via
 * URLs
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class AudioDictionaryURL : public AudioDictionaryBase {
public:
  AudioDictionaryURL(URLStream &urlStream, const char *url) {
    p_url = &urlStream;
    url_cstr = url;
  }

  AudioStream *get(const char *word) {
    url_str = url_cstr;
    url_str.add("/");
    url_str.add(word);
    LOGI("Using url: %s", url_str.c_str());
    p_url->begin(url_str.c_str());
    return p_url;
  }

protected:
  const char *url_cstr;
  StrExt url_str;
  URLStream *p_url;
};

} // namespace simple_tts
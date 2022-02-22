#pragma once
#include "AudioTools/AudioStreams.h"
#include "AudioTools/AudioOutput.h"

namespace simple_tts {

/**
 * @brief We suppress any silince that is longer then the indicated n samples.
 * We assume that we process an T stream on one single channel!
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

template <typename T>
class SilenceRemovalStream : public audio_tools::AudioStreamX {
 public:
  SilenceRemovalStream(AudioStream &out) { this->p_out = &out; }
  SilenceRemovalStream(AudioPrint &out) { this->p_out = &out; }
  ~SilenceRemovalStream() {
    if (buffer != nullptr) {
      delete[] buffer;
    }
  }

  bool begin(int n = 5, int aplidudeLimit = 2) {
    LOGI("begin(n=%d, aplidudeLimit=%d", n, aplidudeLimit);
    this->n = n;
    this->amplidude_limit = aplidudeLimit;
    this->priorLastAudioPos = n+1;  // ignore first values
    this->active = n > 0;
    return true;
  }

  virtual size_t write(const uint8_t *data, size_t size) override {
    if (!active) {
      return p_out->write(data, size);
    }
    size_t sample_count = size / sizeof(T);
    size_t write_count = 0;
    T *audio = (T *)data;

    allocateBuffer(size);

    // find relevant data
    T *p_buffer = (T *)buffer;
    for (int j = 0; j < sample_count; j++) {
      int pos = findLastAudioPos(audio, j);
      if (pos < n) {
        write_count++;
        *p_buffer++ = audio[j];
      }
    }
    
    // write audio data w/o silence
    size_t write_size = write_count * sizeof(T);
    p_out->write(buffer, write_size);
    LOGI("filtered silence from %d -> %d", (int)size, (int)write_size);

    // number of empty samples of prior buffer
    priorLastAudioPos =  findLastAudioPos(audio, sample_count - 1);

    // return processed data size
    return size;
  }

  virtual int availableForWrite() override { return p_out->availableForWrite(); }

  void end() override {
    priorLastAudioPos = 0;
    active = false;
  }

 protected:
  bool active = false;
  Print *p_out = nullptr;
  const uint8_t *buffer = nullptr;
  int n;
  int priorLastAudioPos = 0;
  int amplidude_limit = 0;

  void allocateBuffer(int size) {
    if (buffer == nullptr) {
      buffer = new uint8_t[size];
    }
  }

  // find last position which contains audible data
  int findLastAudioPos(T *audio, int pos) {
    for (int j = 0; j < n; j++) {
      // we are before the start of the current buffer
      if (pos - j <= 0) {
        return priorLastAudioPos;
      }
      // we are in the current buffer
      if (abs(audio[pos - j]) > amplidude_limit) {
        return j;
      }
    }
    return n + 1;
  }
};

}  // namespace simple_tts
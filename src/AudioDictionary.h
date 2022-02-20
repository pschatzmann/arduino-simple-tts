#pragma once

#include "SimpleTTSBase.h"

namespace simple_tts {

#include "en/all.h"

/**
 * @brief A dictionary which is based on in memory audio files
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class AudioDictionary : public AudioDictionaryBase  {
 public:
  AudioDictionary(AudioDictionaryEntry *dict) { dictionary = dict; }
  AudioStream *get(const char *word) {
    if (word==nullptr) return nullptr;
    Str str(word);
    AudioDictionaryEntry *e = dictionary;
    while (e->name != nullptr) {
      if (str.equalsIgnoreCase(e->name)) {
        return e->audio;
      }
      e++;
    }
    return nullptr;
  }

 protected:
  AudioDictionaryEntry *dictionary;
};

/**
 * @brief In Memory collection of words with the related mp3 files
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
AudioDictionaryEntry ExampleAudioDictionaryValues[] = {
    {"AND", new MemoryStream(and_mp3, and_mp3_len)},
    {"BILLION", new MemoryStream(billion_mp3, billion_mp3_len)},
    {"DOT", new MemoryStream(dot_mp3, dot_mp3_len)},
    {"EIGHT", new MemoryStream(eight_mp3, eight_mp3_len)},
    {"EIGHTEEN", new MemoryStream(eighteen_mp3, eighteen_mp3_len)},
    {"EIGHTY", new MemoryStream(eighty_mp3, eighty_mp3_len)},
    {"ELEVEN", new MemoryStream(eleven_mp3, eleven_mp3_len)},
    {"FIFTEEN", new MemoryStream(fifteen_mp3, fifteen_mp3_len)},
    {"FIFTY", new MemoryStream(fifty_mp3, fifty_mp3_len)},
    {"FIVE", new MemoryStream(five_mp3, five_mp3_len)},
    {"FOUR", new MemoryStream(four_mp3, four_mp3_len)},
    {"FOURTEEN", new MemoryStream(fourteen_mp3, fourteen_mp3_len)},
    {"FOURTY", new MemoryStream(fourty_mp3, fourty_mp3_len)},
    {"HUNDRED", new MemoryStream(hundred_mp3, hundred_mp3_len)},
    {"MILLION", new MemoryStream(million_mp3, million_mp3_len)},
    {"MINUS", new MemoryStream(minus_mp3, minus_mp3_len)},
    {"NINE", new MemoryStream(nine_mp3, nine_mp3_len)},
    {"NINETEEN", new MemoryStream(nineteen_mp3, nineteen_mp3_len)},
    {"NINETY", new MemoryStream(ninety_mp3, ninety_mp3_len)},
    {"OCLOCK", new MemoryStream(oclock_mp3, oclock_mp3_len)},
    {"ONE", new MemoryStream(one_mp3, one_mp3_len)},
    {"SEVEN", new MemoryStream(seven_mp3, seven_mp3_len)},
    {"SEVENTEEN", new MemoryStream(seventeen_mp3, seventeen_mp3_len)},
    {"SEVENTY", new MemoryStream(seventy_mp3, seventy_mp3_len)},
    {"SIX", new MemoryStream(six_mp3, six_mp3_len)},
    {"SIXTEEN", new MemoryStream(sixteen_mp3, sixteen_mp3_len)},
    {"SIXTY", new MemoryStream(sixty_mp3, sixty_mp3_len)},
    {"TEN", new MemoryStream(ten_mp3, ten_mp3_len)},
    {"THIRTEEN", new MemoryStream(thirteen_mp3, thirteen_mp3_len)},
    {"THIRTY", new MemoryStream(thirty_mp3, thirty_mp3_len)},
    {"THOUSAND", new MemoryStream(thousand_mp3, thousand_mp3_len)},
    {"THREE", new MemoryStream(three_mp3, three_mp3_len)},
    {"TWELVE", new MemoryStream(twelve_mp3, twelve_mp3_len)},
    {"TWENTY", new MemoryStream(twenty_mp3, twenty_mp3_len)},
    {"TWO", new MemoryStream(two_mp3, two_mp3_len)},
    {"ZERO", new MemoryStream(zero_mp3, zero_mp3_len)},
    // additional entries to support time
    {"AM", new MemoryStream(am_mp3, am_mp3_len)},
    {"HALF", new MemoryStream(half_mp3, half_mp3_len)},
    {"ITS", new MemoryStream(its_mp3, its_mp3_len)},
    {"MIDNIGHT", new MemoryStream(midnight_mp3, midnight_mp3_len)},
    {"NOON", new MemoryStream(noon_mp3, noon_mp3_len)},
    {"PAST", new MemoryStream(past_mp3, past_mp3_len)},
    {"PM", new MemoryStream(pm_mp3, pm_mp3_len)},
    {"QUARTER", new MemoryStream(quarter_mp3, quarter_mp3_len)},
    {"TO", new MemoryStream(to_mp3, to_mp3_len)},

    // must end with nullptr to mark end
    {nullptr, nullptr}

};

}
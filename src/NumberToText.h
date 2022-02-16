#pragma once
#include "SimpleTTSBase.h"
#include "AudioBasic/Vector.h"
#include <stdio.h>
#include <string.h>

namespace simple_tts {

/**
 * @brief Translates a number into englich words
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class NumberToText : public SimpleTTSBase {
 public:

  /// converts a number to it's text representation
  audio_tools::Vector<const char*> &say(double value) {
    result.clear();
    if (value<0){
      add(third[MINUS]);
    }

    // number before decimals
    int intValue = value;
    convert(intValue);

    // add decimals
    double dec = value - intValue;
    decimals(dec);

    // provide result to callback
    if (callback){
        callback(result, reference);
    }

    return result;
  }

  /// provides all texts
  audio_tools::Vector<const char*> &allTexts() {
    result.clear();
    for (int j=0;j<20;j++){
      add(first[j]);
    }
    for (int j=0;j<10;j++){
      add(second[j]);
    }
    for (int j=0;j<6;j++){
      add(third[j]);
    }
    return result;
  }

 protected:
  audio_tools::Vector<const char*> result;
  const char* first[20] = {"ZERO",    "ONE",       "TWO",       "THREE",
                           "FOUR",    "FIVE",      "SIX",       "SEVEN",
                           "EIGHT",   "NINE",      "TEN",       "ELEVEN",
                           "TWELVE",  "THIRTEEN",  "FOURTEEN",  "FIFTEEN",
                           "SIXTEEN", "SEVENTEEN", "EIGHTTEEN", "NINETEEN"};

  const char* second[10] = {"",      "TEN",   "TWENTY",  "THIRTY", "FORTY",
                            "FIFTY", "SIXTY", "SEVENTY", "EIGHTY", "NINETY"};

  const char* third[6] = {"HUNDRED", "THOUSAND", "MILLION","AND","DOT","MINUS"};
  const int HUNDRED = 0;
  const int THOUSAND = 1;
  const int MILLION = 2;
  const int AND = 3;
  const int DOT = 4;
  const int MINUS = 5;

  void add(const char* str){
    result.push_back(str);
  }

  void decimals(double decimals) {
    char str_decimals[50];
    if (decimals!=0.0){
      snprintf(str_decimals,50,"%f", decimals);
      char* ch = strchr(str_decimals,'.');
      if (ch!=nullptr){
        add(third[DOT]);
        while (*++ch){
          int idx = (*ch)-48; // convert to index
          add(first[idx]);
        }
      }
    }
  }

  void convert(int value){
    if (value < 0) {
      convert(-value);
    } else if (value >= 1000000) {
      convert(value / 1000);
      add(third[MILLION]);
      if (value % 1000) {
        if (value % 1000 < 100) {
          add(third[AND]);
        }
        convert(value % 1000);
      }
    } else if (value >= 1000) {
      convert(value / 1000);
      add(third[THOUSAND]);
      if (value % 1000) {
        if (value % 1000 < 100) {
          add(third[AND]);
        }
        convert(value % 1000);
      }
    } else if (value >= 100) {
      convert(value / 100);
      add(third[HUNDRED]);
      if (value % 100) {
        add(third[AND]);
        convert(value % 100);
      }
    } else if (value >= 20) {
      add(second[value / 10]);
      if (value % 10) {
        convert(value % 10);
      }
    } else {
      add(first[value]);
    }
  }
};

}  // namespace simple_tts
#pragma once
#include "SimpleTTSBase.h"
#include "AudioBasic/Vector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace simple_tts {

/**
 * @brief Translates a number into englich words
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class NumberToText : public SimpleTTSBase {
 public:

  /// converts a real number to it's text representation (with the indicated number of digits)
  audio_tools::Vector<const char*> &say(double value, int decimals=2) {
    Number number;
    number.set(value, decimals);
    return say(number.intValue(), number.decValues());
  }

  audio_tools::Vector<const char*> &say(int64_t wholeNumber) {
    Number number;
    number.set(wholeNumber);
    return say(number.intValue(), number.decValues());
  }

  /// converts a number to it's text representation
  audio_tools::Vector<const char*> &say(const char* wholeNumber, const char* decimals="") {
    result.clear();

    LOGI("say(number='%s', decimal='%s')",wholeNumber, decimals);
    Str wn(wholeNumber);
    wn.trim();
    if (wn.startsWith("-")){
      add(third[MINUS]);
    }

    // number before decimals
    convert(wholeNumber);
 
    // add decimals
    if (Str(decimals).toLong()>0l){
      convertDecimals(decimals);
    }

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
    for (int j=0;j<7;j++){
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
                           "SIXTEEN", "SEVENTEEN", "EIGHTEEN",  "NINETEEN"};

  const char* second[10] = {"",      "TEN",   "TWENTY",  "THIRTY", "FORTY",
                            "FIFTY", "SIXTY", "SEVENTY", "EIGHTY", "NINETY"};

  const char* third[7] = {"AND","DOT","MINUS", "HUNDRED", "THOUSAND", "MILLION","BILLION"};
  const int AND = 0;
  const int DOT = 1;
  const int MINUS = 2;
  const int HUNDRED = 3;
  const int THOUSAND = 4;
  const int MILLION = 5;
  const int BILLION = 6;

  void add(const char* str){
    result.push_back(str);
  }

  void convertDecimals(const char* decimals) {
    Str dec(decimals);
    if (dec.toInt()!=0.0){
      const char* ch=decimals;
      add(third[DOT]);
      while (*++ch){
        int idx = (*ch)-48; // convert to index
        add(first[idx]);
      }
    }
  }
  void convert(const char* strValue){
    convert(Str(strValue).toLong());
  }

  // TODO: convert to string operations
  void convert(int64_t value){
    if (value < 0) {
      convert(-value);
    } else if (value >= 1000000000) {
      convert(value / 1000000000);
      add(third[BILLION]);
      int64_t remainder = value%1000000000;
      if (remainder) convert(remainder);
    } else if (value >= 1000000) {
      convert(value / 1000000);
      add(third[MILLION]);
      int64_t remainder = value%1000000;
      if (remainder) convert(remainder);
    } else if (value >= 1000) {
      convert(value / 1000);
      add(third[THOUSAND]);
      int64_t remainder = value%1000;
      if (remainder) convert(remainder);
    } else if (value >= 100) {
      convert(value / 100);
      add(third[HUNDRED]);
      int64_t remainder = value%100;
      if (remainder) convert(remainder);
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
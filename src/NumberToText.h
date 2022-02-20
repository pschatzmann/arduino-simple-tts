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

  /// converts a real number to it's text representation (with the indicated number of digits)
  audio_tools::Vector<const char*> &say(double value, int digits=3) {
    int32_t intValue = value;
    double dec_double = abs(value - intValue);
    uint32_t dec_int = round(dec_double * pow(10,digits+1)) / 10;
    return say(intValue, dec_int);
  }

  audio_tools::Vector<const char*> &say(int16_t wholeNumber, uint16_t decimals=0) {
    return say(static_cast<int32_t>(wholeNumber),static_cast<uint32_t>(decimals));
  }

  /// converts a number to it's text representation
  audio_tools::Vector<const char*> &say(int32_t wholeNumber, uint32_t decimals=0) {
    result.clear();

    LOGI("say(%d,%d)",wholeNumber,decimals);
    if (wholeNumber<0){
      add(third[MINUS]);
    }

    // number before decimals
    convert(wholeNumber);
 
    // add decimals
    if (decimals>0){
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
                           "SIXTEEN", "SEVENTEEN", "EIGHTTEEN", "NINETEEN"};

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

  void convertDecimals(uint16_t decimals) {
    char str_decimals[50];
    if (decimals!=0.0){
      snprintf(str_decimals,50,"%u", decimals);
      char* ch=str_decimals;
      add(third[DOT]);
      while (*++ch){
        int idx = (*ch)-48; // convert to index
        add(first[idx]);
      }
    }
  }

  void convert(int32_t value){
    if (value < 0) {
      convert(-value);
    } else if (value >= 1000000000) {
      convert(value / 1000000000);
      add(third[BILLION]);
      int32_t remainder = value%1000000000;
      if (remainder) convert(remainder);
    } else if (value >= 1000000) {
      convert(value / 1000000);
      add(third[MILLION]);
      int32_t remainder = value%1000000;
      if (remainder) convert(remainder);
    } else if (value >= 1000) {
      convert(value / 1000);
      add(third[THOUSAND]);
      int32_t remainder = value%1000;
      if (remainder) convert(remainder);
    } else if (value >= 100) {
      convert(value / 100);
      add(third[HUNDRED]);
      int32_t remainder = value%100;
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
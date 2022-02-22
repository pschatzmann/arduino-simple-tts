#pragma once
#include "NumberToText.h"

namespace simple_tts {

/**
 * @brief An extension of the Number to Text functionality which supports
 * numbers with units of measures.
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

class NumberUnitToText : public SimpleTTSBase {
 public:
  // Default constructor with no default unit
  NumberUnitToText() = default;

  // Constructor which assigns a default uinit so that we do not need to provide
  // it
  NumberUnitToText(const char* defaultUnit) { default_unit = defaultUnit; }

  /// converts a real number with a unit to it's text representation
  audio_tools::Vector<const char*>& say(double value,
                                        const char* unit = nullptr,
                                        int decimals = 2) {
    Number number;
    number.set(value, decimals);
    return say(number.intValue(), number.decValues(), unit);
  }

  /// converts an integer with a unit to it's text representation
  audio_tools::Vector<const char*>& say(int64_t wholeNumber,
                                        const char* unit = nullptr) {
    Number number;
    number.set(wholeNumber);
    return say(number.intValue(), number.decValues(), unit);
  }

  /// converts a number provided by string components to it's text representation
  audio_tools::Vector<const char*>& say(const char* wholeNumber,
                                        const char* decimals,
                                        const char* unitIn) {
    result.clear();

    // determine unit
    const char* unit = unitIn ? unitIn : default_unit;
    // singular or plural ?
    bool isOne = Str(wholeNumber).toLong() == 1l;

    // We support 2 scenarios: combined units or just a smple number with a single unit.
    int idx = idxCombinedUnit(unit);
    if (idx == -1) {
      // Simple units scenario
      addAll(ntt.say(wholeNumber, decimals));
      process(unit, isOne);
    } else {
      // Combined units ->
      // say the wholeNumber first
      addAll(ntt.say(wholeNumber));
      // say whole number unit
      processCombinedUnit(idx, isOne);
      // combine them with AND
      add("AND");
      // say converted decimals
      Number calc;
      int digits = decimalInfo(unit);
      const char* decimals_to_say = calc.decAsInt(decimals, digits);
      addAll(ntt.say(decimals_to_say));
      // say decimals unit
      bool decIsOne = Str(decimals).toLong() == 1l;
      processDecimalUnit(idx, decIsOne);
    }

    // provide result to callback
    if (callback){
        callback(result, reference);
    }

    return result;
  }

  /// Provides all texts
  virtual audio_tools::Vector<const char*>& allTexts() {
    result.clear();

    // add words from unit_1
    for (int j = 0; j < max_unit_1; j++) {
      add(unit_1[j][1]);
      add(unit_1[j][2]);
    }

    // add words form unit_3
    for (int j = 0; j < max_unit_2; j++) {
      for (int i = 1; i < 4; i++) {
        const char* word = unit2[j][i];
        if (word != nullptr) {
          if (!isKey(word)) {
            add(word);
          }
        }
      }
    }

    return result;
  }

 protected:
  const char* default_unit = nullptr;
  audio_tools::Vector<const char*> result;
  NumberToText ntt;

  // units which can have singurlar and plural
  const static int max_unit_1 = 6;
  const char* unit_1[max_unit_1][3] = {
      {"lb", "pound", "pounds"},  {"ft", "foot", "feet"},
      {"in", "inch", "inches"},   {"mi", "mile", "miles"},
      {"$", "dollar", "dollars"}, {"¢", "cent", "cents"}};

  /// units that can be constructed by combining words
  const static int max_unit_2 = 15;
  const char* unit2[max_unit_2][4] = {{"mph", "miles", "per", "hour"},
                                      {"C", "degrees", "celcius", nullptr},
                                      {"F", "degrees", "fahrenheit", nullptr},
                                      {"%", "percent", nullptr, nullptr},
                                      {"lt", "liter", nullptr, nullptr},
                                      {"ml", "milli", "lt", nullptr},
                                      {"gr", "gram", nullptr, nullptr},
                                      {"kg", "kilo", "gr", nullptr},
                                      {"mg", "milli", "gr", nullptr},
                                      {"km", "kilo", "m", nullptr},
                                      {"m", "meter", nullptr, nullptr},
                                      {"cm", "centi", "m", nullptr},
                                      {"mm", "milli", "m", nullptr},
                                      {"u.s.", "us", nullptr, nullptr},
                                      {"usd", "u.s.", "$", nullptr}};

  // split unit before and after the decimal point: 1.20 USD is 1 dollar and 20
  // cents
  const static int max_unit_3 = 6;
  const char* unit3[max_unit_3][2] = {{"usd", "¢"}, {"gr", "mg"}, {"kg", "gr"},
                                      {"m", "mm"},  {"km", "m"},  {"lt", "ml"}};
  
  /// Units might have different decimals. The decimal system is based on 3. But currencies have usually 2.
  struct DecimalInfo {
    const char* id;
    int dec;
  };
  const static int decimal_info_len = 1;
  const DecimalInfo decimal_info[decimal_info_len] = {{"usd", 2}};

  // add a single word to the result
  void add(const char* str) { result.push_back(str); }

  // add a list to the result
  void addAll(audio_tools::Vector<const char*>& words) {
    for (auto word : words) {
      add(word);
    }
  }

  /// resolve keys until we end up with final words
  bool process(const char* unit, bool isOne) {
    if (!isKey(unit)) {
      add(unit);
      return true;
    }
    // check in table1 first
    if (process1(unit, isOne)) return true;
    // process table 2
    if (process2(unit, isOne)) return true;
    // last resort use the unit directly
    if (!isKey(unit)) {
      LOGE("Error %s", unit);
    }
    return false;
  }

  bool process2(const char* unit, bool isOne) {
    // process table 2
    bool result = false;
    Str u(unit);
    for (int j = 0; j < max_unit_2; j++) {
      if (u.equalsIgnoreCase(unit2[j][0])) {
        for (int i = 1; i < 4; i++) {
          const char* word = unit2[j][i];
          if (word != nullptr) {
            if (!isKey(word)) {
              add(word);
              result = true;
            } else {
              if (process(word, isOne)) {
                result = true;
              }
            }
          }
        }
      }
    }
    return result;
  }

  bool process1(const char* unit, bool isOne) {
    Str u(unit);
    for (int j = 0; j < max_unit_1; j++) {
      if (u.equalsIgnoreCase(unit_1[j][0])) {
        // determine singular or plural
        add(isOne ? unit_1[j][1] : unit_1[j][2]);
        return true;
      }
    }
    return false;
  }

  bool isKey(const char* unit) {
    Str u(unit);
    for (int j = 0; j < max_unit_1; j++) {
      if (u.equalsIgnoreCase(unit_1[j][0])) {
        return true;
      }
    }

    for (int j = 0; j < max_unit_2; j++) {
      if (u.equalsIgnoreCase(unit2[j][0])) {
        return true;
      }
    }
    return false;
  }

  // checks if this is a combined unit (part of unit3) where we need to
  // separate the integers from the decimals with a different unit
  int idxCombinedUnit(const char* unit) {
    Str u(unit);
    for (int i = 0; i < max_unit_3; i++) {
      if (u.equalsIgnoreCase(unit3[i][0])) {
        return i;
      }
    }
    return -1;
  }

  bool processCombinedUnit(int idx, bool isOne) {
    return process(unit3[idx][0], isOne);
  }

  bool processDecimalUnit(int idx, bool isOne) {
    return process(unit3[idx][1], isOne);
  }

  int decimalInfo(const char* unit) {
    Str u(unit);
    for (int i = 0; i < decimal_info_len; i++) {
      if (u.equalsIgnoreCase(decimal_info[i].id)) {
        return decimal_info[i].dec;
      }
    }
    // all metric units are based on 1000
    return 3;
  }
};

}  // namespace simple_tts
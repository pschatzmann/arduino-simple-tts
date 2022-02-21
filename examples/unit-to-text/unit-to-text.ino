
/**
 * @file numberunit-to-text.ino
 * @author Phil Schatzmann
 * @brief Demo that translates numbers with units into individual words. This is used as input to generate speech
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "SimpleTTS.h"

NumberUnitToText utt;

void setup(){
    Serial.begin(115200);

    auto result = utt.say(1.1,"usd");
    for (auto str : result){
        Serial.print(str);
        Serial.print(" ");
    }
}

void loop() {

}
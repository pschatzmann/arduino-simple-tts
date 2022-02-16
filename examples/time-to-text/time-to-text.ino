/**
 * @file time-to-text.ino
 * @author Phil Schatzmann
 * @brief Demo sketch which translates a number into words. This is the input for generating speech.
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "SimpleTTS.h"

TimeToText ttt;

void setup(){
    Serial.begin(115200);

    auto result = ttt.say(12,00);
    
    for (auto str : result){
        Serial.print(str);
        Serial.print(" ");
    }
}

void loop() {

}

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
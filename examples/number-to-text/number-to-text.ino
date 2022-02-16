
#include "SimpleTTS.h"

NumberToText ntt;

void setup(){
    Serial.begin(115200);

    auto result = ntt.say(-700123.431);
    for (auto str : result){
        Serial.print(str);
        Serial.print(" ");
    }
}

void loop() {

}
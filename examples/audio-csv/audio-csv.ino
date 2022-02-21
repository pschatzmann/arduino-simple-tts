
/**
 * @file audio-csv.ino
 * @author Phil Schatzmann
 * @brief We generate the CSV output that can be used as input to the provided tools
 * @version 0.1
 * @date 2022-02-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "SimpleTTS.h"

NumberToText ntt;
TimeToText ttt;

void setup() {
    Serial.begin(115200);
    Serial.println("Name,Text");
    ntt.printCSV(Serial);
    ttt.printCSV(Serial);
}

void loop(){

}
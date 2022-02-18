
/**
 * @file sd-initial-load.ino
 * @author Phil Schatzmann
 * @brief We use Modilla TTS to generate WAV audio files and store them on a SD drive. This demo shows how to 
 * use long texts
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "SimpleTTS.h"
#include "AudioDictionarySD.h"

// collection of audio samples. The first field is the file name, the second is used to generate the audio file
AudioSDEntry entries[]= {
    {"test","test"},
    {"@menu1","This is the main menu: Pleasse say Continue"},
    {"@menu2","This is the ordering menu: Pleasse say A to cancel your order or say B to change the order"},
    // end marker
    {nullptr,nullptr},
};


AudioDictionarySD sd("/tts","wav"); // store the files in the /tts directory with the wav extension.
const char* url="http://pi.local:5002/api/tts?text=@"; // Mozilla TTS on a Raspberry PI
const char* mime="audio/wav";
const char* ssid = "SSID";
const char* password = "password";

void connectWifi() {
    // connect to WiFi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println(" CONNECTED");
}

void setup(){
    Serial.begin(115200);

    connectWifi();
    // load numbers audio
    sd.initialLoad(entries, url, mime);

}

void loop(){

}


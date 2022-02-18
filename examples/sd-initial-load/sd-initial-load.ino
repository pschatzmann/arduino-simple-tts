
/**
 * @file sd-initial-load.ino
 * @author Phil Schatzmann
 * @brief We use Modilla TTS to generate WAV audio files and store them on a SD drive
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "SimpleTTS.h"
#include "AudioDictionarySD.h"


NumberToText ntt; // source for number texts
TimeToText ttt;   // source for time based texts
AudioDictionarySD sd("/tts","wav"); // store the files in the /tts directory with the wav extension.
const char* url="http://pi.local:5002/api/tts?text=@"; // Mozilla TTS on a Raspberry PI
const char* mime="audio/wav";
const char* ssid = "Phil Schatzmann";
const char* password = "sabrina01";

void connectWifi() {
    // connect to WiFi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.println("CONNECTED");
}


void setup(){
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Debug);
    // only for audiokit
    //SPI.begin(PIN_AUDIO_KIT_SD_CARD_CLK, PIN_AUDIO_KIT_SD_CARD_MISO, PIN_AUDIO_KIT_SD_CARD_MOSI, PIN_AUDIO_KIT_SD_CARD_CS);

    connectWifi();
    // load numbers audio
    sd.initialLoad(ntt, url, mime);
    // load time audio
    sd.initialLoad(ttt, url, mime);
    
    Serial.println("END");
}

void loop(){

}

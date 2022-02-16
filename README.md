# Arduino Simple TTS

Microcontrollers do not have enough resources to provide a high quality Text to Speech functionality. 
However often it might be good enough just to provide a solution which is based on some pre-recorded audio.

I was wondering about the limitations of this approach and decided to implement a small prototype Arduino library
which is based on the [Arduino Audio Tools](https://github.com/pschatzmann/arduino-audio-tools) for the audio output.

To keep things simple I decided to start with an simple implementation which can process numbers and on top of that another one which 
reads out the time. So the starting point are some classes wich tranlate numbers to test.

This functionality can be used e.g. to build some

- talking clocks
- talking scales


## Conversion to Text Representation

### Numbers to Text

NumberToText translates the number input into a audio_tools::Vector of words. In the following examples we just print them out:

```
NumberToText ntt;

auto result = ntt.say(700123.431);

for (auto str : result){
    Serial.print(str);
    Serial.print(" ");
}

```

### Time to Text

To process the time you need to provide the hours and minuts as input.

```
TimeToText ttt;

auto result = ttt.say(12, 00);

for (auto str : result){
    Serial.print(str);
    Serial.print(" ");
}
```

## Text to Speech

If we record the words in mp3 we might even get away with the need of a separate SD drive because we can store the audio in program memory.
The following sketch demonstrates how you can use the classes described above to convert them into audio output.

```
#include "SimpleTTS.h"
#include "AudioCodecs/CodecMP3Helix.h"

TimeToText ttt; // Text source
I2SStream i2s;  // audio output via I2S
MP3DecoderHelix mp3;  // mp3 decoder
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(tts, i2s, mp3, dictionary);

void setup(){
    Serial.begin(115200);
    // setup i2s
    auto cfg = i2s.defaultConfig(); 
    i2s.begin(cfg);

    tts.say(14,40);
}

void loop() {
}

```
The ExampleAudioDictionaryValues contains the prerecorded mp3 files which are stored in the progmem.


# Arduino Simple TTS

Microcontrollers do not have enough resources to provide a high quality Text to Speech functionality. 
However often it might be good enough just to provide a solution which is based on some pre-recorded audio.

I was wondering about the limitations of this approach and decided to implement a small prototype Arduino library
which is based on the [Arduino Audio Tools](https://github.com/pschatzmann/arduino-audio-tools) for the audio output.

To keep things simple I decided to start with a simple implementation that can process numbers and on top of that another one which 
reads out the time. So the starting point are some classes that tranlate numbers to test.

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
The result is: SEVEN HUNDRED THOUSAND ONE HUNDRED AND TWENTY THREE DOT FOUR THREE ONE ZERO ZERO ZERO 

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
The result is: NOON

## Text to Speech

If we record the words in mp3 we might even get away with the need of a separate SD drive because we can store the audio in program memory. The ExampleAudioDictionaryValues contains the prerecorded mp3 files which are stored in the progmem.

```
#include "SimpleTTS.h"
#include "AudioCodecs/CodecMP3Helix.h"

I2SStream i2s;  // audio output via I2S
MP3DecoderHelix mp3;  // mp3 decoder
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(tts, i2s, mp3, dictionary);

void setup(){
    Serial.begin(115200);
    // setup i2s
    auto cfg = i2s.defaultConfig(); 
    cfg.sample_rate = 24000;
    cfg.channels = 1;
    i2s.begin(cfg);

    tts.say("BILLION");
}

void loop() {
}

```
The word "Billion" is spoken out via I2S.

You can also use the text generation classes described above to convert them into audio output:

```
#include "SimpleTTS.h"
#include "AudioCodecs/CodecMP3Helix.h"

TimeToText ttt; // Text source
I2SStream i2s;  // audio output via I2S
MP3DecoderHelix mp3;  // mp3 decoder
AudioDictionary dictionary(ExampleAudioDictionaryValues);
TextToSpeech tts(i2s, mp3, dictionary);

void setup(){
    Serial.begin(115200);
    // setup i2s
    auto cfg = i2s.defaultConfig(); 
    cfg.sample_rate = 24000;
    cfg.channels = 1;
    i2s.begin(cfg);

    tts.say(14,40);
}

void loop() {
}

```

## Memory Usage

Here is the info for sketch which provides talking time and number support and stores all audio files as mp3 in progmem on as ESP32:
```
Sketch uses 740438 bytes (23%) of program storage space. Maximum is 3145728 bytes.
Global variables use 23632 bytes (7%) of dynamic memory, leaving 304048 bytes for 
```

I think this leave plenty of headroom and you still have the option to store the audio on an SD drive...


## Documentation

Here is the [link to the generated class documentation](https://pschatzmann.github.io/arduino-simple-tts/docs/html/annotated.html)
Further information can be found in the [Wiki](https://github.com/pschatzmann/arduino-simple-tts/wiki) and in my [Blogs](https://www.pschatzmann.ch/home/tag/tts/)

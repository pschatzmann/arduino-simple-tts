#pragma once
/**
 * @brief Some special rules for desktop builds. 
 * The output should go to PortAudioStream
 */

#ifdef IS_DESKTOP
#include "AudioLibs/PortAudioStream.h"
#define I2SStream PortAudioStream 
#define AudioKitStream PortAudioStream
#endif

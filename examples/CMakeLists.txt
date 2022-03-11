cmake_minimum_required(VERSION 3.16)

# set the project name
project(simple-tts-examples)

include(FetchContent)

# Build with helix 
FetchContent_Declare(helix GIT_REPOSITORY "https://github.com/pschatzmann/arduino-libhelix.git" GIT_TAG main )
FetchContent_GetProperties(helix)
if(NOT helix_POPULATED)
    FetchContent_Populate(helix)
    add_subdirectory(${helix_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/helix)
endif()

# Build with arduino-audio-tools
FetchContent_Declare(audio_tools GIT_REPOSITORY "https://github.com/pschatzmann/arduino-audio-tools.git" GIT_TAG main )
FetchContent_GetProperties(audio_tools)
if(NOT audio_tools_POPULATED)
    FetchContent_Populate(audio_tools)
    add_subdirectory(${audio_tools_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/audio_tools)
endif()

target_compile_definitions(arduino-audio-tools INTERFACE -DARDUINO -DEXIT_ON_STOP -DIS_DESKTOP )

add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/number-to-text")
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/time-to-text")
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/time-to-speech")
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/unit-to-text")
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/number-to-speech")
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/unit-to-speech")
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/audio-i2s-nosilence")
# not supported (yet?)
#add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/example-own-sound")
#add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/time-to-speech-sd")
#add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/talking-clock")

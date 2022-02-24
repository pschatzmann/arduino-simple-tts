cmake_minimum_required(VERSION 3.20)

# set the project name
project(audio-i2s-nosilence)
set (CMAKE_CXX_STANDARD 11)
set (DCMAKE_CXX_FLAGS "-Werror")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

include(FetchContent)

# build sketch as executable
add_executable (audio-i2s-nosilence audio-i2s-nosilence.ino ../main.cpp)
# defines needed for desktop build
target_compile_definitions(audio-i2s-nosilence PUBLIC -DARDUINO -DEXIT_ON_STOP -DIS_DESKTOP -DUSE_HELIX)
# define ino as cpp
set_source_files_properties(audio-i2s-nosilence.ino PROPERTIES LANGUAGE CXX)
# specify libraries
target_link_libraries(audio-i2s-nosilence arduino_emulator arduino_helix arduino-audio-tools simple-tts portaudio)


cmake_minimum_required(VERSION 3.20)

# set the project name
project(pw)
set (CMAKE_CXX_STANDARD 11)
set (DCMAKE_CXX_FLAGS "-Werror")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

# build sketch as executable
add_executable (tts tts.ino ../main.cpp)
# defines needed for desktop build
target_compile_definitions(tts PUBLIC -DARDUINO -DEXIT_ON_STOP -DIS_DESKTOP -DNO_SPEECH)
# define ino as cpp
set_source_files_properties(tts.ino PROPERTIES LANGUAGE CXX)
# specify libraries
target_link_libraries(tts arduino_emulator arduino-audio-tools arduino_helix simple-tts)


cmake_minimum_required(VERSION 3.20)

# set the project name
project(unit-to-text)
set (CMAKE_CXX_STANDARD 11)
set (DCMAKE_CXX_FLAGS "-Werror")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

# build sketch as executable
add_executable (unit-to-text unit-to-text.ino ../main.cpp)
# defines needed for desktop build
target_compile_definitions(unit-to-text PUBLIC -DARDUINO -DEXIT_ON_STOP -DIS_DESKTOP -DNO_SPEECH)
# define ino as cpp
set_source_files_properties(unit-to-text.ino PROPERTIES LANGUAGE CXX)
# specify libraries
target_link_libraries(unit-to-text arduino_emulator arduino-audio-tools simple-tts)


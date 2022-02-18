cmake_minimum_required(VERSION 3.20)

# set the project name
project(sd-initial-load-long)
set (CMAKE_CXX_STANDARD 11)
set (DCMAKE_CXX_FLAGS "-Werror")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

include(FetchContent)

# build sketch as executable
add_executable (sd-initial-load-long sd-initial-load-long.ino ../main.cpp)
# defines needed for desktop build
target_compile_definitions(sd-initial-load-long PUBLIC -DARDUINO -DEXIT_ON_STOP -DIS_DESKTOP -DNO_SPEECH)
# add SD 
target_include_directories(sd-initial-load-long PUBLIC ${arduino_emulator_SOURCE_DIR}/ArduinoCore-Linux/libraries/SD)
# define ino as cpp
set_source_files_properties(sd-initial-load-long.ino PROPERTIES LANGUAGE CXX)
# specify libraries
target_link_libraries(sd-initial-load-long simple-tts arduino_emulator arduino-audio-tools )


cmake_minimum_required(VERSION 3.16)

# set the project name
project(simple-tts)

# lots of warnings and all warnings as errors
## add_compile_options(-Wall -Wextra )
set(CMAKE_CXX_STANDARD 17)

# prevent compile errors
#target_compile_options(simple-tts PRIVATE -DIS_DESKTOP)
add_library(simple-tts INTERFACE)

# define location for header files
target_include_directories(simple-tts INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src )

# build examples
#add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/examples" EXCLUDE_FROM_ALL)
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/examples" )

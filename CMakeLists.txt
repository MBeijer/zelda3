cmake_minimum_required(VERSION 3.23)
project(zelda3)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES "*.c" "*.cpp")
file(GLOB HEADERS *.h)
file(GLOB SNES_SOURCES "snes/*.c" "snes/*.cpp")
file(GLOB GL_CORE_SOURCES "third_party/gl_core/*.h" "third_party/gl_core/*.cpp")
file(GLOB OPUS_SOURCES "third_party/opus-*/opus_decoder_amalgam.c")

find_package(SDL2 REQUIRED)

add_executable(zelda3 ${SOURCES} ${SNES_SOURCES} ${GL_CORE_SOURCES} ${OPUS_SOURCES} ${HEADERS})

target_link_libraries(zelda3 PRIVATE SDL2::SDL2)
target_compile_options(zelda3 PUBLIC -Wno-narrowing -fpermissive)

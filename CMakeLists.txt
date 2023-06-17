cmake_minimum_required(VERSION 3.23)
project(zelda3)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES "*.c" "*.cpp")
file(GLOB HEADERS *.h)
file(GLOB SNES_SOURCES "snes/*.c" "snes/*.cpp")
file(GLOB GL_CORE_SOURCES "third_party/gl_core/*.h" "third_party/gl_core/*.cpp")
file(GLOB OPUS_SOURCES "third_party/opus-*/opus_decoder_amalgam.c")

find_package(SDL 1.2 REQUIRED)

if(WIN32)
    file(GLOB PLATFORM_SOURCES "platform/win32/volume_control.cpp" "platform/win32/*.h")
else()
    set(PLATFORM_SOURCES "")
endif()

message("asd: ${PLATFORM_SOURCES}")

add_executable(zelda3 ${SOURCES} ${SNES_SOURCES} ${GL_CORE_SOURCES} ${OPUS_SOURCES} ${PLATFORM_SOURCES} ${HEADERS})

if(WIN32)
    target_include_directories(zelda3 PRIVATE platform/win32)
endif()

target_link_libraries(zelda3 PRIVATE ${SDL_LIBRARIES})
target_include_directories(zelda3 PRIVATE ${SDL_INCLUDE_DIRS})
target_compile_options(zelda3 PUBLIC -Wno-narrowing -fpermissive)

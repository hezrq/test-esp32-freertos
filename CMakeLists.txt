cmake_minimum_required(VERSION 3.13)

project(test_esp32)

add_executable(myapp src/main.c)

set(IDF_PROJECT_EXECUTABLE myapp)

set(AFR_BOARD espressif.esp32_devkitc CACHE INTERNAL "")
add_subdirectory(freertos)


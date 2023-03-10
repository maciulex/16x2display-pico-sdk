cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(pico_examples C CXX ASM)

pico_sdk_init()

add_executable(display16x2
        main.cpp
    )

pico_enable_stdio_usb(display16x2 1)
pico_enable_stdio_uart(display16x2 0)

target_link_libraries(display16x2 pico_stdlib hardware_i2c) #hardware_spi hardware_i2c
pico_add_extra_outputs(display16x2)
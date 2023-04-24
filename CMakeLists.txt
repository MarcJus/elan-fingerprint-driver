cmake_minimum_required(VERSION 3.24)
project(fingerprint_reader C)

set(CMAKE_C_STANDARD 11)

add_executable(fingerprint_reader fingerprint.c test.c fingerprint.h fingerprint_usb_request.c fingerprint_usb_request.h)
include_directories(/usr/src/linux-headers-5.19.0-40-generic/include)
include_directories(/usr/src/linux-headers-5.19.0-40-generic/arch/x86/include/)
include_directories(/usr/src/linux-headers-5.19.0-40-generic/arch/x86/include/generated)

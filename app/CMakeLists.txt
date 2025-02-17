#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

# /app level CMakeLists.txt
cmake_minimum_required(VERSION 3.20.0)

set(BOARD_RUNNER jlink)
set(BOARD arduino_nano_33_ble/nrf52840/sense)
set(DTC_OVERLAY_FILE ${CMAKE_SOURCE_DIR}/../boards/arm/arduino_nano_33_sense/nano33sense.overlay)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C CXX)

target_sources(app PRIVATE src/main.cpp )

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib/etl_setup)
zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/etl_type)

#EOF /app level
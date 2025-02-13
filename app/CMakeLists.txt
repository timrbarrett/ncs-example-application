#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(BOARD_RUNNER jlink)
set(BOARD arduino_nano_33_ble/nrf52840/sense)
#set(DTC_OVERLAY_FILE boards/arm/arduino_nano_33_sense/nano33sense.overlay)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C)

target_sources(app PRIVATE src/main.c)

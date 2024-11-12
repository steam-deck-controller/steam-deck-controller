cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico_w)
set(PICO_SDK_FETCH_FROM_GIT on)
set(PIO_USB_FETCH_FROM_GIT on)

# initialize the pico SDK
include(pico_sdk_import.cmake)
pico_sdk_init()

# init the pio_usb lib
include(pio_usb_import.cmake)

project(steam_deck_controller)

# rest of your project
add_subdirectory(freertos)
add_subdirectory(transmitter)
add_subdirectory(receiver)

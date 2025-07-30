add_executable(transmitter
        transmitter.c
)
target_link_libraries(transmitter
        pico_stdlib              # for core functionality
        pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything else
        freertos
)

pico_enable_stdio_usb(transmitter 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(transmitter)

# add url via pico_set_program_url
#example_auto_set_url(transmitter)

add_executable(receiver
        receiver.c
)
target_link_libraries(receiver
        pico_stdlib              # for core functionality
        pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything else
        freertos
)

# create map/bin/hex file etc.
pico_add_extra_outputs(receiver)

# add url via pico_set_program_url
#example_auto_set_url(receiver)

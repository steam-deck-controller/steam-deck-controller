add_executable(receiver)

pico_enable_stdio_usb(receiver 1)
target_sources(receiver PRIVATE
        receiver.c)

target_include_directories(receiver PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/receiver.gatt")
    pico_btstack_make_gatt_header(receiver PRIVATE ${CMAKE_CURRENT_LIST_DIR}/receiver.gatt)
endif()

target_link_libraries(receiver
        pico_stdlib              # for core functionality
        pico_multicore
        freertos
#        tinyusb_device
#        tinyusb_host
#        tinyusb_board
        pico_pio_usb
        pico_btstack_ble
#        pico_btstack_classic
        pico_cyw43_arch_none
#        pico_lwip_freertos
        pico_btstack_cyw43
        pico_cyw43_driver
#        pico_cyw43_arch_lwip_sys_freertos
        #        pico_cyw43_arch_sys_freertos
#        FreeRTOS-Kernel-Heap4
)
target_compile_options(receiver PRIVATE -Wall -Wextra)

target_compile_definitions(receiver PRIVATE
        CYW43_TASK_STACK_SIZE=1024
        NO_SYS=0
)

# create map/bin/hex file etc.
pico_add_extra_outputs(receiver)

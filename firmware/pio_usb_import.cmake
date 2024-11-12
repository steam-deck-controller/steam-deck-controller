# pio_usb_import.cmake
set(PIO_USB_URL https://github.com/sekigon-gonnoc/Pico-PIO-USB.git)
set(PIO_USB_TAG main)  # Or specify a specific version/tag

if (DEFINED ENV{PIO_USB_PATH} AND (NOT PIO_USB_PATH))
    set(PIO_USB_PATH $ENV{PIO_USB_PATH})
    message("Using PIO_USB_PATH from environment ('${PIO_USB_PATH}')")
endif ()

if (NOT PIO_USB_PATH)
    # If FETCH_FROM_GIT is not defined, assume we're trying to pick up a local copy
    if (NOT DEFINED PIO_USB_FETCH_FROM_GIT)
        set(PIO_USB_FETCH_FROM_GIT FALSE)
    endif()

    if (PIO_USB_FETCH_FROM_GIT)
        include(FetchContent)
        FetchContent_Declare(
                pio_usb
                GIT_REPOSITORY ${PIO_USB_URL}
                GIT_TAG ${PIO_USB_TAG}
        )

        if (NOT pio_usb_POPULATED)
            message("Downloading PIO USB library")
            FetchContent_Populate(pio_usb)
            set(PIO_USB_PATH ${pio_usb_SOURCE_DIR})
        endif()
    else()
        message(FATAL_ERROR
                "PIO USB path not defined and FETCH_FROM_GIT not set. Please set PIO_USB_PATH or set PIO_USB_FETCH_FROM_GIT to download from git")
    endif()
endif()

set(PIO_USB_PATH ${PIO_USB_PATH} CACHE PATH "Path to PIO USB library")
add_subdirectory(${PIO_USB_PATH} pio_usb)
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>

#include "hardware/clocks.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "pio_usb.h"

// Use tinyUSB header to define USB descriptors
#include "device/usbd.h"
#include "class/hid/hid_device.h"

static usb_device_t *usb_device = NULL;

// USB Device Descriptor
tusb_desc_device_t const desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0110,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = 64,
    .idVendor = 0xCafe,
    .idProduct = 0,
    .bcdDevice = 0x0100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01
};

enum {
    ITF_NUM_KEYBOARD,
    ITF_NUM_MOUSE,
    ITF_NUM_TOTAL,
};

enum {
    EPNUM_KEYBOARD = 0x81,
    EPNUM_MOUSE = 0x82,
};

uint8_t const desc_hid_keyboard_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD()
};

uint8_t const desc_hid_mouse_report[] = {
    TUD_HID_REPORT_DESC_MOUSE()
};

const uint8_t *report_desc[] = {
    desc_hid_keyboard_report,
    desc_hid_mouse_report
};

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + 2*TUD_HID_DESC_LEN)
uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN,
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_HID_DESCRIPTOR(ITF_NUM_KEYBOARD, 0, HID_ITF_PROTOCOL_KEYBOARD,
                       sizeof(desc_hid_keyboard_report), EPNUM_KEYBOARD,
                       CFG_TUD_HID_EP_BUFSIZE, 10),
    TUD_HID_DESCRIPTOR(ITF_NUM_MOUSE, 0, HID_ITF_PROTOCOL_MOUSE,
                       sizeof(desc_hid_mouse_report), EPNUM_MOUSE,
                       CFG_TUD_HID_EP_BUFSIZE, 10),
};

static_assert(sizeof(desc_device) == 18, "device desc size error");

const char *string_descriptors_base[] = {
    [0] = (const char[]){0x09, 0x04},
    [1] = "Pico PIO USB",
    [2] = "Pico PIO USB Device",
    [3] = "123456",
};
static string_descriptor_t str_desc[4];

static void init_string_desc(void) {
    for (int idx = 0; idx < 4; idx++) {
        uint8_t len = 0;
        uint16_t *wchar_str = (uint16_t *)&str_desc[idx];
        if (idx == 0) {
            wchar_str[1] = string_descriptors_base[0][0] |
                          ((uint16_t)string_descriptors_base[0][1] << 8);
            len = 1;
        } else if (idx <= 3) {
            len = strnlen(string_descriptors_base[idx], 31);
            for (int i = 0; i < len; i++) {
                wchar_str[i + 1] = string_descriptors_base[idx][i];
            }
        } else {
            len = 0;
        }
        wchar_str[0] = (TUSB_DESC_STRING << 8) | (2 * len + 2);
    }
}

static usb_descriptor_buffers_t desc = {
    .device = (uint8_t *)&desc_device,
    .config = desc_configuration,
    .hid_report = report_desc,
    .string = str_desc
};

// Task argument structure for LED blinking
struct led_task_arg {
    int gpio;
    int delay;
};

// LED blink task
void led_task(void *p) {
    struct led_task_arg *a = (struct led_task_arg *)p;

    while (true) {
        cyw43_arch_gpio_put(a->gpio, 1);
        vTaskDelay(pdMS_TO_TICKS(a->delay));
        printf("LED on\n");
        cyw43_arch_gpio_put(a->gpio, 0);
        vTaskDelay(pdMS_TO_TICKS(a->delay));
        printf("LED off\n");
    }
}

// USB device initialization and management task
void usb_device_task(void *p) {
    printf("Initializing USB device...\n");

    // Initialize USB device
    static pio_usb_configuration_t config = PIO_USB_DEFAULT_CONFIG;
    init_string_desc();
    usb_device = pio_usb_device_init(&config, &desc);

    if (usb_device == NULL) {
        printf("Failed to initialize USB device\n");
        vTaskDelete(NULL);
        return;
    }

    printf("USB device initialized successfully\n");

    // Main USB device task loop
    while (true) {
        pio_usb_device_task();
        // Give other tasks a chance to run
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Mouse movement task
void mouse_task(void *p) {
    // Wait a bit for USB device to initialize
    vTaskDelay(pdMS_TO_TICKS(2000));

    while (true) {
        if (usb_device != NULL) {
            hid_mouse_report_t mouse_report = {0};
            mouse_report.x = 1;
            endpoint_t *ep = pio_usb_get_endpoint(usb_device, 2);
            if (ep != NULL) {
                pio_usb_set_out_data(ep, (uint8_t *)&mouse_report, sizeof(mouse_report));
                printf("Mouse moved\n");
            } else {
                printf("Mouse endpoint not available\n");
            }
        } else {
            printf("USB device not ready\n");
        }
        vTaskDelay(pdMS_TO_TICKS(500)); // Move mouse every 500ms
    }
}

int main() {
    // Set appropriate system clock (multiple of 12MHz for USB)
    set_sys_clock_khz(120000, true);

    stdio_init_all();
    printf("Starting FreeRTOS USB HID Device with LED blink\n");

    // Initialize Wi-Fi chip for LED control
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    printf("Wi-Fi chip initialized for LED control\n");

    // Create all tasks to run on the same core under FreeRTOS management

    // Create LED blink task
    struct led_task_arg led_arg = { CYW43_WL_GPIO_LED_PIN, 1000 };
    BaseType_t led_result = xTaskCreate(led_task, "LED_Task", 256, &led_arg, 1, NULL);
    if (led_result != pdPASS) {
        printf("Failed to create LED task\n");
        return -1;
    }

    // Create USB device task with higher priority
    BaseType_t usb_result = xTaskCreate(usb_device_task, "USB_Device_Task", 1024, NULL, 3, NULL);
    if (usb_result != pdPASS) {
        printf("Failed to create USB device task\n");
        return -1;
    }

    // Create mouse movement task
    BaseType_t mouse_result = xTaskCreate(mouse_task, "Mouse_Task", 512, NULL, 2, NULL);
    if (mouse_result != pdPASS) {
        printf("Failed to create mouse task\n");
        return -1;
    }

    printf("All tasks created successfully, starting scheduler\n");

    // Start FreeRTOS scheduler
    vTaskStartScheduler();

    // Should never reach here
    printf("ERROR: Scheduler returned!\n");
    while (true) {
        tight_loop_contents();
    }
}
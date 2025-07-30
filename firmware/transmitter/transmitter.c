/*
 * LED blink with FreeRTOS
 */

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

struct led_task_arg {
    int gpio;
    int delay;
};

void led_task(void *p)
{
    struct led_task_arg *a = (struct led_task_arg *)p;

    while (true) {
        cyw43_arch_gpio_put(a->gpio, 1);
        vTaskDelay(pdMS_TO_TICKS(a->delay));
        printf("Turning on\n");
        cyw43_arch_gpio_put(a->gpio, 0);
        vTaskDelay(pdMS_TO_TICKS(a->delay));
        printf("Turning off\n");
    }
}

int main()
{
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    printf("Start LED blink\n");

    struct led_task_arg arg1 = { CYW43_WL_GPIO_LED_PIN, 1000 };
    xTaskCreate(led_task, "LED_Task 1", 256, &arg1, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}

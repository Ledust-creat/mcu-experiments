#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "lv_port.h"
#include "ui_home.h"
#include "ui_rgb.h"
void app_main(void)
{
    lv_port_init();
    ui_home_create();
    //ui_rgb_creat();
    while (1)
    {
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
        
    }
    
}

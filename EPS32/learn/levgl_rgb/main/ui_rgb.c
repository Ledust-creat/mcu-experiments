#include "driver/gpio.h"
#include "esp_log.h"
#include "lvgl.h"
#include "led_ws2812.h"
static lv_obj_t * s_ui_rgb_button = NULL;
static lv_obj_t * s_ui_rgb_label = NULL;
ws2812_strip_handle_t ws2812_handle = NULL;
void lv_ui_rgb_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
       ws2812_write(ws2812_handle,0,4,4,46);
        break;
    
    default:
        break;
    }
}
void ui_rgb_creat(void)
{
    

    ws2812_init(GPIO_NUM_38,1,&ws2812_handle);
    ws2812_write(ws2812_handle,0,46,4,4);

    lv_obj_set_style_bg_color(lv_scr_act(),lv_palette_main(LV_PALETTE_LIGHT_BLUE),0);

    s_ui_rgb_button = lv_obj_create(lv_scr_act());
    lv_obj_align(s_ui_rgb_button,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_bg_color(s_ui_rgb_button,lv_palette_main(LV_PALETTE_YELLOW),0);
    lv_obj_set_size(s_ui_rgb_button,60,60);

    s_ui_rgb_label = lv_label_create(s_ui_rgb_button);
    lv_obj_align(s_ui_rgb_label,LV_ALIGN_CENTER,0,0);
    lv_label_set_text(s_ui_rgb_label,"RGB");

    lv_obj_add_event_cb(s_ui_rgb_button,lv_ui_rgb_cb,LV_EVENT_CLICKED,NULL);
}
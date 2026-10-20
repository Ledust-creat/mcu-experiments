#include "lvgl.h"
#include "led_ws2812.h"
#include "dht11.h"
#include "esp_log.h"

LV_IMG_DECLARE(temp_img)
LV_IMG_DECLARE(humidity_img)

#define WS2812_NUM        1
#define WS2812_GPIO_NUM   38
#define DHT11_GPIO_NUM    18

static lv_obj_t* s_temp_image;
static lv_obj_t* s_humidity_image;

static lv_obj_t* s_temp_label;
static lv_obj_t* s_humidity_label;


static lv_obj_t *s_slider_r;  // 红色滑块
static lv_obj_t *s_slider_g;  // 绿色滑块
static lv_obj_t *s_slider_b;  // 蓝色滑块
static uint8_t s_rgb_value[3] = {0}; // 存储RGB值

static lv_timer_t* s_dht11_timer;

ws2812_strip_handle_t ws2812_handle;

void light_slider_event_cb(lv_event_t * e)
{
    
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
        case LV_EVENT_VALUE_CHANGED:
        {       
            lv_obj_t* slider_obj = lv_event_get_target(e);
            int32_t value = lv_slider_get_value(slider_obj);
            // 判断是哪个滑块并更新对应通道值
            if (slider_obj == s_slider_r) {
                s_rgb_value[0] = (uint8_t)value;
            } else if (slider_obj == s_slider_g) {
                s_rgb_value[1] = (uint8_t)value;
            } else if (slider_obj == s_slider_b) {
                s_rgb_value[2] = (uint8_t)value;
            }
            
            for(int led_index = 0 ;led_index < WS2812_NUM;led_index++)
            {
                ws2812_write(ws2812_handle,led_index,s_rgb_value[0],s_rgb_value[1],s_rgb_value[2]);
            }
            break;
        }
        default:
            break;
    }
}

void dht11_timer_cb(struct _lv_timer_t *t)
{
    int temp;
    int humidity;
    if(DHT11_StartGet(&temp,&humidity))
    {
        char disp_buf[32];
        snprintf(disp_buf,sizeof(disp_buf),"%.1f",(float)temp/10.0);
        lv_label_set_text(s_temp_label,disp_buf);

        snprintf(disp_buf,sizeof(disp_buf),"%d%%",humidity);
        lv_label_set_text(s_humidity_label,disp_buf);
        
    }
}

void ui_home_create(void)
{
    //初始WS2812
    ws2812_init(WS2812_GPIO_NUM,WS2812_NUM,&ws2812_handle);

    //DHT11
    DHT11_Init(DHT11_GPIO_NUM);

    //创建定时器
    s_dht11_timer = lv_timer_create(dht11_timer_cb,2000,NULL);

    //背景设置黑色
    lv_obj_set_style_bg_color(lv_scr_act(),lv_color_black(),0);

    //创建温度图片
    s_temp_image = lv_img_create(lv_scr_act());
    lv_img_set_src(s_temp_image,&temp_img);
    lv_obj_set_pos(s_temp_image,10,20);

    //创建湿度图片
    s_humidity_image = lv_img_create(lv_scr_act());
    lv_img_set_src(s_humidity_image,&humidity_img);
    lv_obj_set_pos(s_humidity_image,120,25);

    //创建温度label
    s_temp_label = lv_label_create(lv_scr_act());
    lv_obj_set_pos(s_temp_label,70,40);
    lv_obj_set_style_text_font(s_temp_label,&lv_font_montserrat_26,0);

    //创建湿度label
    s_humidity_label = lv_label_create(lv_scr_act());
    lv_obj_set_pos(s_humidity_label,180,40);
    lv_obj_set_style_text_font(s_humidity_label,&lv_font_montserrat_26,0);
    
    //创建调光用的进度条
    s_slider_r = lv_slider_create(lv_scr_act());
    lv_obj_set_pos(s_slider_r,60,100);
    lv_obj_set_size(s_slider_r,150,15);
    lv_slider_set_range(s_slider_r,0,255);
    lv_obj_add_event_cb(s_slider_r,light_slider_event_cb,LV_EVENT_VALUE_CHANGED,NULL);
    lv_obj_set_style_bg_color(s_slider_r, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
    lv_obj_set_style_bg_color(s_slider_r, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(s_slider_r, lv_palette_main(LV_PALETTE_RED),  LV_PART_KNOB);  

    s_slider_g = lv_slider_create(lv_scr_act());
    lv_obj_set_pos(s_slider_g,60,145);
    lv_obj_set_size(s_slider_g,150,15);
    lv_slider_set_range(s_slider_g,0,255);
    lv_obj_add_event_cb(s_slider_g,light_slider_event_cb,LV_EVENT_VALUE_CHANGED,NULL);
    lv_obj_set_style_bg_color(s_slider_g, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
    lv_obj_set_style_bg_color(s_slider_g, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(s_slider_g, lv_palette_main(LV_PALETTE_GREEN),  LV_PART_KNOB); 

    s_slider_b = lv_slider_create(lv_scr_act());
    lv_obj_set_pos(s_slider_b,60,190);
    lv_obj_set_size(s_slider_b,150,15);
    lv_slider_set_range(s_slider_b,0,255);
    lv_obj_add_event_cb(s_slider_b,light_slider_event_cb,LV_EVENT_VALUE_CHANGED,NULL);
    lv_obj_set_style_bg_color(s_slider_b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_set_style_bg_color(s_slider_b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(s_slider_b, lv_palette_main(LV_PALETTE_BLUE),  LV_PART_KNOB); 


}
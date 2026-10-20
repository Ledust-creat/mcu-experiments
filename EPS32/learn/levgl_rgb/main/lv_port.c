#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "lv_port.h"
#include "lvgl.h"
#include "st7789_driver.h"
#include "cst816t_driver.h"

static lv_disp_drv_t disp_drv;
static const char *TAG = "lv_port";

#define LCD_WIDTH   240
#define LCD_HEIGHT  240
/*
1.初始化st7789硬件驱动
2.初始化和注册lvgl显示驱动
3.初始化cst816t硬件驱动
4.初始化和注册lvgl触摸驱动
5.提供一个定时器给lvgl  
*/
/**
 * @brief LVGL定时器时钟
 *
 * @param pvParam 无用
 */
static void lv_tick_inc_cb(void *data)
{
    uint32_t tick_inc_period_ms = *((uint32_t *) data);
    lv_tick_inc(tick_inc_period_ms);
}

/**
 * @brief 通知LVGL写入数据完毕
 */
static void lv_port_flush_ready(void* param)
{
    lv_disp_flush_ready(&disp_drv);

    /* portYIELD_FROM_ISR (true) or not (false). */
}


/**
 * @brief 写入显示数据
 *
 * @param disp_drv  对应的显示器
 * @param area      显示区域
 * @param color_p   显示数据
 */
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    (void) disp_drv;
    st7789_flush(area->x1 , area->x2 + 1 , area->y1,area->y2 + 1, color_p);
}

/**
 * @brief 注册LVGL显示驱动
 *
 */
static void lv_port_disp_init(void)
{
    static lv_disp_draw_buf_t draw_buf_dsc;
    size_t disp_buf_height = LCD_WIDTH*(LCD_HEIGHT/6);

    /* 必须从内部RAM分配显存，这样刷新速度快 */
    lv_color_t *p_disp_buf1 = heap_caps_malloc(disp_buf_height * sizeof(lv_color_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
    lv_color_t *p_disp_buf2 = heap_caps_malloc(disp_buf_height * sizeof(lv_color_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
    ESP_LOGI(TAG, "Try allocate two  %u display buffer, size:%u Byte",  disp_buf_height,  disp_buf_height * sizeof(lv_color_t) * 2);
    if (NULL == p_disp_buf1 || NULL == p_disp_buf2) {
        ESP_LOGE(TAG, "No memory for LVGL display buffer");
        esp_system_abort("Memory allocation failed");
    }

    /* 初始化显示缓存 */
    lv_disp_draw_buf_init(&draw_buf_dsc, p_disp_buf1, p_disp_buf2, disp_buf_height);

    /* 初始化显示驱动 */
    lv_disp_drv_init(&disp_drv);

    /*设置水平和垂直宽度*/
    disp_drv.hor_res = LCD_WIDTH;       //水平宽度
    disp_drv.ver_res = LCD_HEIGHT;      //垂直宽度

    /* 设置刷新数据函数 */
    disp_drv.flush_cb = disp_flush;

    /*设置显示缓存*/
    disp_drv.draw_buf = &draw_buf_dsc;

    /*注册显示驱动*/
    lv_disp_drv_register(&disp_drv);
}


/**
 * @brief 获取触摸坐标
 *
 * @param indev_drv  触摸驱动
 * @param data      数据
 * @return 无
 */
void IRAM_ATTR indev_read(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    int16_t x,y; 
    int state;
    cst816t_read(&x,&y,&state);
    //data->point.x = y;
    //if(x == 0)
    //    x = 1;
    //data->point.y = LCD_HEIGHT - x;
    data->point.x=x;
    data->point.y=y;
    data->state = state;
   
}

/**
 * @brief 注册LVGL输入驱动
 *
 * @return esp_err_t
 */
static esp_err_t lv_port_indev_init(void)
{
    static lv_indev_drv_t   indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = indev_read;
    lv_indev_drv_register(&indev_drv);
    return ESP_OK;
}

/**
 * @brief 创建LVGL定时器
 *
 * @return esp_err_t
 */
static esp_err_t lv_port_tick_init(void)
{
    static uint32_t tick_inc_period_ms = 5;
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = lv_tick_inc_cb,
        .name = "",
        .arg = &tick_inc_period_ms,
        .dispatch_method = ESP_TIMER_TASK,
        .skip_unhandled_events = true,
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, tick_inc_period_ms * 1000));
    return ESP_OK;
}

/**
 * @brief LCD接口初始化
 *
 * @return NULL
 */
static void lcd_init(void)
{
    st7789_cfg_t st7789_config;
    st7789_config.mosi = GPIO_NUM_13;
    st7789_config.clk = GPIO_NUM_14;
    st7789_config.cs = GPIO_NUM_15;
    st7789_config.dc = GPIO_NUM_2;
    st7789_config.rst = GPIO_NUM_4;
    st7789_config.bl = GPIO_NUM_10;
    st7789_config.spi_fre = 40*1000*1000;       //SPI时钟频率
    st7789_config.width = LCD_WIDTH;            //屏宽
    st7789_config.height = LCD_HEIGHT;          //屏高
    st7789_config.spin = 0;                     //顺时针旋转90度
    st7789_config.done_cb = lv_port_flush_ready;    //数据写入完成回调函数
    st7789_config.cb_param = &disp_drv;         //回调函数参数

    st7789_driver_hw_init(&st7789_config);
}

/**
 * @brief 触摸芯片初始化
 *
 * @return NULL
 */
static void tp_init(void)
{
    cst816t_cfg_t cst816t_config;
    cst816t_config.sda = GPIO_NUM_5;
    cst816t_config.scl = GPIO_NUM_6;
    //cst816t_config.x_limit = LCD_HEIGHT;    //由于屏幕显示旋转了90°，X和Y触摸需要调转
    //cst816t_config.y_limit = LCD_WIDTH;
    cst816t_config.x_limit = LCD_WIDTH;
    cst816t_config.y_limit = LCD_HEIGHT;
    cst816t_config.fre = 200*1000;
    

    cst816t_init(&cst816t_config);
}


esp_err_t lv_port_init(void)
{
    /* 初始化LVGL库 */
    lv_init();

    /*lcd接口初始化*/
    lcd_init();

    /* 注册显示驱动 */
    lv_port_disp_init();

    /*触摸芯片初始化*/
    tp_init();

    /* 注册输入驱动*/
    lv_port_indev_init();

    /* 初始化LVGL定时器 */
    lv_port_tick_init();

    return ESP_OK;
}

idf_component_register(
    SRCS
        "cst816t_driver.c " "st7789_driver.c" "led_ws2812.c" "dht11.c"
    INCLUDE_DIRS
        "."
    REQUIRES
        esp_timer
        lvgl
        main)
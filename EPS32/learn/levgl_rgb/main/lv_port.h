/**
 * @file lv_port.h
 * @brief 
 * @version 0.1
 * @date 2026-02-02
 * 
 * @copyright 
 */

#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Init LVGL GUI library
 * 
 * @return
 *    - ESP_OK: Success
 */
esp_err_t lv_port_init(void);

#ifdef __cplusplus
}
#endif

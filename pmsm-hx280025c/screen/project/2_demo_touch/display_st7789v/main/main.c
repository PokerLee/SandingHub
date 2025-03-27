#include <stdio.h>

#include "lv_port_disp.h"
#include "lv_port_indev.h"

// demo
#include "lv_demo_benchmark.h"
// #include "lv_demo_keypad_encoder.h"
#include "lv_demo_music.h"
#include "lv_demo_stress.h"
#include "lv_demo_widgets.h"

extern void example_lvgl_demo_ui(lv_disp_t *disp);

bool example_lvgl_lock(int timeout_ms)
{
    // Convert timeout in milliseconds to FreeRTOS ticks
    // If `timeout_ms` is set to -1, the program will block until the condition is met
    const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}

void example_lvgl_unlock(void)
{
    xSemaphoreGiveRecursive(lvgl_mux);
}

static void example_lvgl_port_task(void *arg)
{
    ESP_LOGI(TAG, "Starting LVGL task");
    uint32_t task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
    while (1)
    {
        // Lock the mutex due to the LVGL APIs are not thread-safe
        if (example_lvgl_lock(-1))
        {
            task_delay_ms = lv_timer_handler();
            // Release the mutex
            example_lvgl_unlock();
        }
        if (task_delay_ms > EXAMPLE_LVGL_TASK_MAX_DELAY_MS)
        {
            task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
        }
        else if (task_delay_ms < EXAMPLE_LVGL_TASK_MIN_DELAY_MS)
        {
            task_delay_ms = EXAMPLE_LVGL_TASK_MIN_DELAY_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

// 定时回调函数，每 1ms 触发
static void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

// 初始化 LVGL Tick 定时器
void lvgl_tick_timer_init(void) 
{
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &example_increase_lvgl_tick,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "lvgl_tick"};
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));
}

void app_main(void)
{
    static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
    static lv_disp_drv_t disp_drv;      // contains callback functions

    ESP_LOGI(TAG, "Install LVGL tick timer");
    lvgl_tick_timer_init();

    ESP_LOGI(TAG, "Initialize LVGL library");
    lv_init();

    ESP_LOGI(TAG, "Initialize LVGL display");
    lv_port_disp_init();

    ESP_LOGI(TAG, "Initialize LVGL touch");
    lv_port_indev_init();

    // lvgl_mux = xSemaphoreCreateRecursiveMutex();
    // assert(lvgl_mux);
    // ESP_LOGI(TAG, "Create LVGL task");
    // xTaskCreate(example_lvgl_port_task, "LVGL", EXAMPLE_LVGL_TASK_STACK_SIZE, NULL, EXAMPLE_LVGL_TASK_PRIORITY, NULL);
    // ESP_LOGI(TAG, "Display LVGL Meter Widget");

    // // Lock the mutex due to the LVGL APIs are not thread-safe
    // /* code */
    // if (example_lvgl_lock(-1))
    // {
    //     //example_lvgl_demo_ui(disp);
    //     lv_demo_widgets();
    //     // Release the mutex
    //     example_lvgl_unlock();
    // }

    /* Demo_0 */
    // /*在屏幕中间创建一个120*50大小的按钮*/
    // lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
    // lv_obj_set_size(switch_obj, 120, 50);
    // lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);
    
    /* Demo_1 */
    // lv_demo_benchmark();
    // lv_demo_music();
    // lv_demo_stress();
    lv_demo_widgets();

    while (1) 
    {
        lv_task_handler();  // LVGL 任务管理
        vTaskDelay(pdMS_TO_TICKS(10));  // 延迟 10ms
    }
}

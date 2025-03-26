#include <stdio.h>
#include "lv_port_disp.h"

extern void example_lvgl_demo_ui(lv_disp_t *disp);


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

    lv_port_disp_init();

    // ESP_LOGI(TAG, "Register display driver to LVGL");
    // lv_disp_drv_init(&disp_drv);
    // disp_drv.hor_res = EXAMPLE_LCD_H_RES;
    // disp_drv.ver_res = EXAMPLE_LCD_V_RES;
    // disp_drv.flush_cb = example_lvgl_flush_cb;
    // disp_drv.drv_update_cb = example_lvgl_port_update_callback;
    // disp_drv.draw_buf = &disp_buf;
    // disp_drv.user_data = panel_handle;
    // lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
        
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


    /*在屏幕中间创建一个120*50大小的按钮*/
    lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
    lv_obj_set_size(switch_obj, 120, 50);
    lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);
    
    while (1) 
    {
        lv_task_handler();  // LVGL 任务管理
        vTaskDelay(pdMS_TO_TICKS(10));  // 延迟 10ms
    }
}

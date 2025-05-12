/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include "esp_log.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void ta_event_cb(lv_event_t * e);
static void kb_event_cb(lv_event_t * e);  // 键盘事件回调函数
static void close_keyboard();  // 收回键盘

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * kb;
static lv_obj_t * ta;  // 保存文本框对象，用来绑定事件

/**
 * 键盘事件回调函数
 * 用于处理按下"√"或空白处时收回键盘
 */
static void kb_event_cb(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    uint16_t btn_id = 0;  // 定义按钮 ID

    // 遍历所有按钮，获取按钮文本
    for(btn_id = 0; btn_id < 20; btn_id++)
    {
        const char * btn_text = lv_btnmatrix_get_btn_text(btn, btn_id);
        if (btn_text != NULL) {
            ESP_LOGI("keyboard", "Button %d text: %s", btn_id, btn_text);  // 打印按钮文本
        }
    }

    // 获取当前按钮的文本
    // const char * btn_text = lv_btnmatrix_get_btn_text(btn, 7);
    btn_id = lv_btnmatrix_get_selected_btn(btn);
    ESP_LOGI("keyboard", "Button %d has been pressed ", (int)btn_id);
    if(btn_id == 7)
    {
        close_keyboard();
        return;
    }
    
    // // 检查是否点击了"√"按钮 1006987712
    // if ((btn_text != NULL) && ((int)btn_text == 1006987712)) {
    //     close_keyboard();
    //     return;
    // }
}

/**
 * 关闭键盘
 */
static void close_keyboard()
{
    if (kb != NULL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);  // 隐藏键盘
    }
}

/**
 * 文本框事件回调函数
 * 用于处理文本框被点击时弹出键盘
 */
static void ta_event_cb(lv_event_t * e)
{
    lv_obj_t * ta = lv_event_get_target(e);
    const char * txt = lv_textarea_get_text(ta);
    if(txt[0] >= '0' && txt[0] <= '9' &&
       txt[1] >= '0' && txt[1] <= '9' &&
       txt[2] != ':') {
        lv_textarea_set_cursor_pos(ta, 2);
        lv_textarea_add_char(ta, ':');
    }

    // 显示键盘
    if (kb != NULL) {
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);  // 显示键盘
    }
}

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    
    /* 创建文本框并添加事件 */
    ta = ui->screen_ta_1;  // 获取文本框对象
    lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_FOCUSED, NULL);  // 添加聚焦事件回调，点击文本框弹出键盘

    /*Create a keyboard*/
    kb = lv_keyboard_create(lv_scr_act());
    lv_obj_set_size(kb,  LV_HOR_RES, LV_VER_RES / 2);
    lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
    lv_keyboard_set_textarea(kb, ta);

    /* 设置键盘按键事件回调，点击"√"或空白区域时收起键盘 */
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_CLICKED, NULL);

    /* 隐藏键盘，直到文本框被点击 */
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    /* 设置键盘弹出和收回的控制逻辑 */
    // lv_obj_add_event_cb(ta, close_keyboard, LV_EVENT_CLICKED, NULL);  // 点击文本框时弹出键盘
}


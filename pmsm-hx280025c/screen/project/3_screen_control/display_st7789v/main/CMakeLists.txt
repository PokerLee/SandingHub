# 设置源代码目录和包含目录
set(src_dirs 
    .
    ./com
    ./lv_port
    ./lv_ui
    ./lv_ui/custom
    ./lv_ui/generated
    ./lv_ui/generated/guider_customer_fonts
    ./lv_ui/generated/guider_fonts
    ./lv_ui/generated/images
    ../components/lvgl/demos/benchmark
    ../components/lvgl/demos/benchmark/assets
    ../components/lvgl/demos/keypad_encoder
    ../components/lvgl/demos/music
    ../components/lvgl/demos/music/assets
    ../components/lvgl/demos/stress
    ../components/lvgl/demos/widgets
    ../components/lvgl/demos/widgets/assets
)

set(include_dirs 
    .
    ./com
    ./lv_port
    ./lv_ui
    ./lv_ui/custom 
    ./lv_ui/generated 
    ./lv_ui/generated/guider_customer_fonts
    ./lv_ui/generated/guider_fonts
    ./lv_ui/generated/images
    ../components/lvgl/demos/benchmark
    ../components/lvgl/demos/benchmark/assets
    ../components/lvgl/demos/keypad_encoder
    ../components/lvgl/demos/music
    ../components/lvgl/demos/music/assets
    ../components/lvgl/demos/stress
    ../components/lvgl/demos/widgets
    ../components/lvgl/demos/widgets/assets
)


set(priv_requires
                    driver
                    esp_timer
                    esp_driver_ledc
                    esp_lcd
                    bsp
                    lvgl
)

# 注册组件
idf_component_register(SRC_DIRS ${src_dirs} INCLUDE_DIRS ${include_dirs} PRIV_REQUIRES ${priv_requires})

# 设置编译选项
component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)

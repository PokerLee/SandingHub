# 设置源代码目录和包含目录
set(src_dirs 
    .
    ./lv_port
)

set(include_dirs 
    .
    ./lv_port
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

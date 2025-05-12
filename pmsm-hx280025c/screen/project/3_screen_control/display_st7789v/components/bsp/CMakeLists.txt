set(src_dirs         
            st7789_driver
            gt911_driver
)
 
set(include_dirs            
            st7789_driver
            gt911_driver
)
 
set(requires
            driver
            esp_timer
            esp_driver_ledc
            esp_lcd
)
 
idf_component_register(
                        SRC_DIRS ${src_dirs}
                        INCLUDE_DIRS ${include_dirs} 
                        REQUIRES ${requires}
)
 
component_compile_options(
                            -ffast-math 
                            -O3
                            -Wno-error=format=-Wno-format
                            -Werror=implicit-function-declaration
)
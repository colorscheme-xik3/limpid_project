idf_component_register(SRCS  "LMPD_adc/ADC_ads.c" "main.c" "LMPD_blt/BLT_spp.c" "LMPD_ch/MSG_handler.c"
                       INCLUDE_DIRS ".")

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")

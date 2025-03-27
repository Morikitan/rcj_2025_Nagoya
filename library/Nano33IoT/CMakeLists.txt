add_library(Nano33IoT Nano33IoT.c)

pico_set_program_name(Nano33IoT "Nano33IoT")
pico_set_program_version(Nano33IoT "0.1")


target_include_directories(Nano33IoT PRIVATE ../actions)


target_link_libraries(Nano33IoT pico_stdlib hardware_uart hardware_adc)
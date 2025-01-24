add_library(actions actions.c)

#pico_set_program_name(actions "actions")
#pico_set_program_version(actions "0.1")

target_include_directories(actions PRIVATE ../motor)
target_include_directories(actions PRIVATE ../line)
target_include_directories(actions PRIVATE ../gyro)
target_include_directories(actions PRIVATE ../Nano33IoT)
target_include_directories(actions PRIVATE ../camera)

target_link_libraries(actions pico_stdlib hardware_gpio)
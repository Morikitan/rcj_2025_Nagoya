add_library(defence defence.c)

pico_set_program_name(defence "defence")
pico_set_program_version(defence "0.1")

target_include_directories(defence PRIVATE ../motor)
target_include_directories(defence PRIVATE ../actions)
target_include_directories(defence PRIVATE ../gyro)
target_include_directories(defence PRIVATE ../Nano33IoT)
target_include_directories(defence PRIVATE ../line)

target_link_libraries(defence pico_stdlib hardware_pwm hardware_gpio)
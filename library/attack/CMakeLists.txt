add_library(attack attack.c)

pico_set_program_name(attack "attack")
pico_set_program_version(attack "0.1")

target_include_directories(attack PRIVATE ../motor)
target_include_directories(attack PRIVATE ../actions)
target_include_directories(attack PRIVATE ../gyro)
target_include_directories(attack PRIVATE ../Nano33IoT)
target_include_directories(attack PRIVATE ../line)
target_include_directories(attack PRIVATE ../camera)
target_include_directories(attack PRIVATE ../defence)

target_link_libraries(attack pico_stdlib hardware_pwm hardware_gpio)
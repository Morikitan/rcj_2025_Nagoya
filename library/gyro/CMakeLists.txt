add_library(gyro gyro.c)

pico_set_program_name(gyro "gyro")
pico_set_program_version(gyro "0.1")

target_link_libraries(gyro pico_stdlib hardware_i2c hardware_gpio)
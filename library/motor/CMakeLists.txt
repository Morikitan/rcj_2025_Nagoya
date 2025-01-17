add_library(motor motor.c)

pico_set_program_name(motor "motor")
pico_set_program_version(motor "0.1")

target_link_libraries(motor pico_stdlib hardware_pwm)
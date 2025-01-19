add_library(attack attack.c)

pico_set_program_name(attack "attack")
pico_set_program_version(attack "0.1")

target_include_directories(attack PRIVATE ../motor)
target_include_directories(attack PRIVATE ../actions)
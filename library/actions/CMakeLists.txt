add_library(actions actions.c)

pico_set_program_name(actions "actions")
pico_set_program_version(actions "0.1")

target_include_directories(actions PRIVATE ../motor)
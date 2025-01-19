add_library(defence defence.c)

pico_set_program_name(defence "defence")
pico_set_program_version(defence "0.1")

target_include_directories(defence PRIVATE ../motor)
target_include_directories(defence PRIVATE ../actions)
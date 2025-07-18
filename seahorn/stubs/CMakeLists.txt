add_library(fd_log_override fd_log_override.c)
sea_attach_bc(fd_log_override)

add_subdirectory(tango)
add_subdirectory(waltz)
add_subdirectory(libc)
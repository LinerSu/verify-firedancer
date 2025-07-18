add_library(ucontext_override ucontext_override.c)
sea_attach_bc(ucontext_override)

add_library(error_override error_override.c)
sea_attach_bc(error_override)

add_library(cstr_override cstr_override.c)
sea_attach_bc(cstr_override)
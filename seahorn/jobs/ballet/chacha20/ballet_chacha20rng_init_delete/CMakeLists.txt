add_executable(ballet_chacha20rng_init_delete
  ${FIREDANCER_ROOT}/src/ballet/chacha20/fd_chacha20rng.c
  ${FIREDANCER_ROOT}/src/ballet/chacha20/fd_chacha20.c
  ballet_chacha20rng_init_delete_harness.c
)
target_include_directories(ballet_chacha20rng_init_delete
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(ballet_chacha20rng_init_delete PRIVATE
  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_chacha20rng.h"
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_link_libraries(ballet_chacha20rng_init_delete fd_log_override.ir)
sea_attach_bc_link(ballet_chacha20rng_init_delete)
sea_add_unsat_test(ballet_chacha20rng_init_delete)
add_executable(tango_tcache_bounds_insert tango_tcache_bounds_insert_harness.c
    ${FIREDANCER_ROOT}/src/tango/tcache/fd_tcache.c
)
target_include_directories(tango_tcache_bounds_insert
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(tango_tcache_bounds_insert PRIVATE
  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_tcache.h"
)
sea_link_libraries(tango_tcache_bounds_insert fd_log_override.ir)
sea_overlink_libraries(tango_tcache_bounds_insert tcache_override.ir)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(tango_tcache_bounds_insert)
sea_add_unsat_test(tango_tcache_bounds_insert)
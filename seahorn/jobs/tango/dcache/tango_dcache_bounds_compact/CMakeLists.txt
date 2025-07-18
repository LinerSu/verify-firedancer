add_executable(tango_dcache_bounds_compact tango_dcache_bounds_compact_harness.c
    ${FIREDANCER_ROOT}/src/tango/dcache/fd_dcache.c
)
target_include_directories(tango_dcache_bounds_compact
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(tango_dcache_bounds_compact PRIVATE
  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_dcache_private.h"
)
sea_link_libraries(tango_dcache_bounds_compact fd_log_override.ir)
sea_overlink_libraries(tango_dcache_bounds_compact dcache_override.ir)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(tango_dcache_bounds_compact)
sea_add_unsat_test(tango_dcache_bounds_compact)
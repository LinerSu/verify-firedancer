add_executable(tango_mcache_bounds_init_delete tango_mcache_bounds_init_delete_harness.c
    ${FIREDANCER_ROOT}/src/tango/mcache/fd_mcache.c
)
target_include_directories(tango_mcache_bounds_init_delete
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(tango_mcache_bounds_init_delete PRIVATE
  # for C files, GCC/Clang will #include this first:
  "-include${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_mcache.h"
  "-include${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_mcache_private.h"
)
sea_link_libraries(tango_mcache_bounds_init_delete fd_log_override.ir)
sea_overlink_libraries(tango_mcache_bounds_init_delete mcache_override.ir)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(tango_mcache_bounds_init_delete)
sea_add_unsat_test(tango_mcache_bounds_init_delete)
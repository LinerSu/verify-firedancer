add_executable(tango_fctl_bounds_init_delete
  ${FIREDANCER_ROOT}/src/tango/fctl/fd_fctl.c
  tango_fctl_bounds_init_delete_harness.c
)
target_include_directories(tango_fctl_bounds_init_delete
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(tango_fctl_bounds_init_delete PRIVATE
  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_fctl.h"
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_link_libraries(tango_fctl_bounds_init_delete fd_log_override.ir)
sea_overlink_libraries(tango_fctl_bounds_init_delete fctl_override.ir)
sea_attach_bc_link(tango_fctl_bounds_init_delete)
sea_add_unsat_test(tango_fctl_bounds_init_delete)
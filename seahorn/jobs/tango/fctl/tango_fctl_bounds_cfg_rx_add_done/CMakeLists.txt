add_executable(tango_fctl_bounds_cfg_rx_add_done
  ${FIREDANCER_ROOT}/src/tango/fctl/fd_fctl.c
  tango_fctl_bounds_cfg_rx_add_done_harness.c
)
target_include_directories(tango_fctl_bounds_cfg_rx_add_done
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(tango_fctl_bounds_cfg_rx_add_done PRIVATE
  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_fctl.h"
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_link_libraries(tango_fctl_bounds_cfg_rx_add_done fd_log_override.ir)
sea_overlink_libraries(tango_fctl_bounds_cfg_rx_add_done fctl_override.ir)
sea_attach_bc_link(tango_fctl_bounds_cfg_rx_add_done)
sea_add_unsat_test(tango_fctl_bounds_cfg_rx_add_done)
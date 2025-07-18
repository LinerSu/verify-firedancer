add_executable(tango_fseq_bounds_init_delete
  ${FIREDANCER_ROOT}/src/tango/fseq/fd_fseq.c
  ${CMAKE_SOURCE_DIR}/seahorn/stubs/tango/fseq_explicit_mem.c
  tango_fseq_bounds_init_delete_harness.c
)
target_include_directories(tango_fseq_bounds_init_delete
  BEFORE PRIVATE
  "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(tango_fseq_bounds_init_delete PRIVATE

  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_fseq.h"
)
sea_link_libraries(tango_fseq_bounds_init_delete fd_log_override.ir)
sea_overlink_libraries(tango_fseq_bounds_init_delete fseq_override.ir)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(tango_fseq_bounds_init_delete)
sea_add_unsat_test(tango_fseq_bounds_init_delete)
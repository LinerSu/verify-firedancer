add_executable(tango_fseq_bounds_query
  ${FIREDANCER_ROOT}/src/tango/fseq/fd_fseq.c
  ${CMAKE_SOURCE_DIR}/seahorn/stubs/tango/fseq_explicit_mem.c
  tango_fseq_bounds_query_harness.c
)
target_include_directories(tango_fseq_bounds_query
  BEFORE PRIVATE
  "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(tango_fseq_bounds_query PRIVATE

  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_fseq.h"
)
sea_link_libraries(tango_fseq_bounds_query fd_log_override.ir)
sea_overlink_libraries(tango_fseq_bounds_query fseq_override.ir)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(tango_fseq_bounds_query)
sea_add_unsat_test(tango_fseq_bounds_query)
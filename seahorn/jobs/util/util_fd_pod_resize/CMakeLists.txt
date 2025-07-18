add_executable(util_fd_pod_resize
    ${FIREDANCER_ROOT}/src/util/pod/fd_pod.c
    util_fd_pod_resize_harness.c
)
target_include_directories(util_fd_pod_resize
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(util_fd_pod_resize PRIVATE
  # for C files, GCC/Clang will #include this first:
  "-include${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_bits.h"
  "-include${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_pod.h"
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(util_fd_pod_resize)
sea_add_unsat_test(util_fd_pod_resize)
add_library(tcache_override tcache_override.c)
target_include_directories(tcache_override
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(tcache_override PRIVATE
  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_tcache.h"
)
sea_attach_bc(tcache_override)

add_library(fseq_override fseq_override.c)
target_include_directories(fseq_override
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(fseq_override PRIVATE
  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_fseq.h"
)
sea_attach_bc(fseq_override)

add_library(dcache_override dcache_override.c)
target_include_directories(dcache_override
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(dcache_override PRIVATE
  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_dcache_private.h"
)
sea_attach_bc(dcache_override)

add_library(mcache_override mcache_override.c)
target_include_directories(mcache_override
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(mcache_override PRIVATE
  # for C files, GCC/Clang will #include this first:
  "-include${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_mcache.h"
  "-include${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_mcache_private.h"
)
sea_attach_bc(mcache_override)

add_library(fctl_override fctl_override.c)
target_include_directories(fctl_override
  BEFORE PRIVATE
    "${CMAKE_SOURCE_DIR}/seahorn/include/stubs"
)
target_compile_options(fctl_override PRIVATE
  # for C files, GCC/Clang will #include this first:
  -include "${CMAKE_SOURCE_DIR}/seahorn/include/stubs/fd_fctl.h"
)
sea_attach_bc(fctl_override)
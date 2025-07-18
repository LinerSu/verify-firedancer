cmake_minimum_required(VERSION 3.18.4)
project(verify-firedancer C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/llvmir)
include(LLVMIRUtil)
include(CTest)

enable_testing()

# Enables LLVM builtin functions for memory operations. We do not check these for memory safety.
# To check for memory safety, set this option to OFF. Then our wrappers will be used instead that
# check for memory safety and call builtin functions directly.
option(SEA_ENABLE_CRAB "Enable Seahorn using Crab" OFF)
option(SEA_ALLOCATOR_CAN_FAIL "Use can fail allocator" OFF)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(
    FATAL_ERROR
    "In-source builds are not allowed. Please clean your source tree and try again."
  )
endif()

# Default is release with debug info
if(NOT CMAKE_BUILD_TYPE)
  set(
    CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE
  )
endif()

set(SEAHORN_ROOT "/usr" CACHE PATH "Path to SeaHorn installation")
set(SEA_LINK "llvm-link" CACHE STRING "Path to llvm-link")
set(LLVMIR_LINK ${SEA_LINK})
set(SEA_OPT "${SEAHORN_ROOT}/bin/seaopt" CACHE STRING "Path to seaopt binary")
set(SEA_PP "${SEAHORN_ROOT}/bin/seapp" CACHE STRING "Path to seapp binary")
set(LLVMIR_OPT ${SEA_OPT})

set(FIREDANCER_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/firedancer)

set(SEA_LIB ${CMAKE_CURRENT_SOURCE_DIR}/seahorn/lib)

configure_file(verify.py.in verify @ONLY)
set(VERIFY_CMD ${CMAKE_CURRENT_BINARY_DIR}/verify)


# add the include directory from the build tree
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/include)

include_directories(seahorn/include)
include_directories(${FIREDANCER_ROOT}/src)
include_directories(${SEAHORN_ROOT}/include)
add_subdirectory(yaml)
add_subdirectory(seahorn)

function(sea_llvm_link name)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs LINK OVERRIDE)
  cmake_parse_arguments(LLVMIR_ATTACH
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT LLVMIR_ATTACH_LINK)
    message(FATAL_ERROR "sea_llvm_link: missing LINK option")
  endif()

  set(LLVMIR_TARGETS ${LLVMIR_ATTACH_LINK})
  set(LLVMIR_OVERRIDE_TARGETS ${LLVMIR_ATTACH_OVERRIDE})

  set(TRGT ${name})
  # use the first target as the main dependency
  list(GET LLVMIR_TARGETS 0 DEPENDS_TRGT)


    ## preamble
  foreach(IR ${LLVMIR_TARGETS})
    llvmir_check_target_properties(${IR})
  endforeach()

  set(OUT_LLVMIR_FILES "")
  set(FULL_OUT_LLVMIR_FILES "")

  get_property(IN_LLVMIR_TYPE TARGET ${DEPENDS_TRGT} PROPERTY LLVMIR_TYPE)
  get_property(LLVMIR_EXTERNAL_TYPE TARGET ${DEPENDS_TRGT}
    PROPERTY LLVMIR_EXTERNAL_TYPE)
  get_property(INFILES TARGET ${DEPENDS_TRGT} PROPERTY LLVMIR_FILES)
  get_property(IN_LLVMIR_DIR TARGET ${DEPENDS_TRGT} PROPERTY LLVMIR_DIR)
  get_property(LINKER_LANGUAGE TARGET ${DEPENDS_TRGT} PROPERTY LINKER_LANGUAGE)
  get_property(LINK_DEPENDS TARGET ${DEPENDS_TRGT} PROPERTY LINK_DEPENDS)
  get_property(LINK_FLAGS TARGET ${DEPENDS_TRGT} PROPERTY LINK_FLAGS)
  get_property(LINK_FLAGS_${CMAKE_BUILD_TYPE}
    TARGET ${DEPENDS_TRGT}
    PROPERTY LINK_FLAGS_${CMAKE_BUILD_TYPE})
  get_property(INTERFACE_LINK_LIBRARIES
    TARGET ${DEPENDS_TRGT}
    PROPERTY INTERFACE_LINK_LIBRARIES)
  get_property(LINK_LIBRARIES TARGET ${DEPENDS_TRGT} PROPERTY LINK_LIBRARIES)
  get_property(LINK_INTERFACE_LIBRARIES
    TARGET ${DEPENDS_TRGT}
    PROPERTY LINK_INTERFACE_LIBRARIES)
  get_property(LINK_INTERFACE_LIBRARIES_${CMAKE_BUILD_TYPE}
    TARGET ${DEPENDS_TRGT}
    PROPERTY LINK_INTERFACE_LIBRARIES_${CMAKE_BUILD_TYPE})
  get_property(SHORT_NAME TARGET ${DEPENDS_TRGT} PROPERTY LLVMIR_SHORT_NAME)

  if(NOT "${IN_LLVMIR_TYPE}" STREQUAL "${LLVMIR_BINARY_TYPE}")
    message(FATAL_ERROR "Cannot attach ${TRGT} to a ${IN_LLVMIR_TYPE} target.")
  endif()

  ## main operations
  set(WORK_DIR "${CMAKE_CURRENT_BINARY_DIR}/${LLVMIR_DIR}/${TRGT}")
  file(MAKE_DIRECTORY ${WORK_DIR})

  set(IN_FULL_LLVMIR_FILES "")
  foreach(IR ${LLVMIR_TARGETS})
    get_property(INFILES TARGET ${IR} PROPERTY LLVMIR_FILES)
    get_property(IN_LLVMIR_DIR TARGET ${IR} PROPERTY LLVMIR_DIR)
    foreach(IN_LLVMIR_FILE ${INFILES})
      list(APPEND IN_FULL_LLVMIR_FILES "${IN_LLVMIR_DIR}/${IN_LLVMIR_FILE}")
    endforeach()
  endforeach()

  get_property(IN_LLVMIR_LINK TARGET ${DEPENDS_TRGT} PROPERTY LLVMIR_LINK)
  foreach(IR ${IN_LLVMIR_LINK})
    get_property(INFILES TARGET ${IR} PROPERTY LLVMIR_FILES)
    get_property(IN_LLVMIR_DIR TARGET ${IR} PROPERTY LLVMIR_DIR)
    foreach(IN_LLVMIR_FILE ${INFILES})
      list(APPEND IN_FULL_LLVMIR_FILES "${IN_LLVMIR_DIR}/${IN_LLVMIR_FILE}")
    endforeach()
  endforeach()


  set(IN_FULL_LLVMIR_OVERIDE_FILES "")
  foreach(IR ${LLVMIR_OVERRIDE_TARGETS})
    get_property(INFILES TARGET ${IR} PROPERTY LLVMIR_FILES)
    get_property(IN_LLVMIR_DIR TARGET ${IR} PROPERTY LLVMIR_DIR)
    foreach(IN_LLVMIR_FILE ${INFILES})
      list(APPEND IN_FULL_LLVMIR_OVERRIDE_FILES "${IN_LLVMIR_DIR}/${IN_LLVMIR_FILE}")
    endforeach()
  endforeach()

  get_property(IN_LLVMIR_OVERRIDE TARGET ${DEPENDS_TRGT} PROPERTY LLVMIR_OVERRIDE)
  foreach(IR ${IN_LLVMIR_OVERRIDE})
    get_property(INFILES TARGET ${IR} PROPERTY LLVMIR_FILES)
    get_property(IN_LLVMIR_DIR TARGET ${IR} PROPERTY LLVMIR_DIR)
    foreach(IN_LLVMIR_FILE ${INFILES})
      list(APPEND IN_FULL_LLVMIR_OVERRIDE_FILES "${IN_LLVMIR_DIR}/${IN_LLVMIR_FILE}")
    endforeach()
  endforeach()

  set(IN_OVERRIDE_OPTIONS ${IN_FULL_LLVMIR_OVERRIDE_FILES})
  list(TRANSFORM IN_OVERRIDE_OPTIONS PREPEND "--override=" )

  set(FULL_OUT_LLVMIR_FILE "${WORK_DIR}/${TRGT}.${LLVMIR_BINARY_FMT_SUFFIX}")
  if(SHORT_NAME)
    set(FULL_OUT_LLVMIR_FILE
      "${WORK_DIR}/${SHORT_NAME}.${LLVMIR_BINARY_FMT_SUFFIX}")
  endif()
  get_filename_component(OUT_LLVMIR_FILE ${FULL_OUT_LLVMIR_FILE} NAME)

  list(APPEND OUT_LLVMIR_FILES ${OUT_LLVMIR_FILE})
  list(APPEND FULL_OUT_LLVMIR_FILES ${FULL_OUT_LLVMIR_FILE})

  # setup custom target
  add_custom_target(${TRGT} DEPENDS ${FULL_OUT_LLVMIR_FILES})

  set_property(TARGET ${TRGT} PROPERTY LLVMIR_TYPE ${LLVMIR_BINARY_TYPE})
  set_property(TARGET ${TRGT}
    PROPERTY LLVMIR_EXTERNAL_TYPE ${LLVMIR_EXTERNAL_TYPE})
  set_property(TARGET ${TRGT} PROPERTY LLVMIR_DIR ${WORK_DIR})
  set_property(TARGET ${TRGT} PROPERTY LLVMIR_FILES ${OUT_LLVMIR_FILES})
  set_property(TARGET ${TRGT} PROPERTY LINKER_LANGUAGE ${LINKER_LANGUAGE})
  set_property(TARGET ${TRGT} PROPERTY LINK_DEPENDS ${LINK_DEPENDS})
  set_property(TARGET ${TRGT} PROPERTY LINK_FLAGS ${LINK_FLAGS})
  set_property(TARGET ${TRGT}
    PROPERTY LINK_FLAGS_${CMAKE_BUILD_TYPE} ${LINK_FLAGS_${CMAKE_BUILD_TYPE}})
  set_property(TARGET ${TRGT}
    PROPERTY INTERFACE_LINK_LIBRARIES ${INTERFACE_LINK_LIBRARIES})
  set_property(TARGET ${TRGT}
    PROPERTY LINK_INTERFACE_LIBRARIES ${LINK_INTERFACE_LIBRARIES})
  set_property(TARGET ${TRGT}
    PROPERTY
    LINK_INTERFACE_LIBRARIES_${CMAKE_BUILD_TYPE}
    ${LINK_INTERFACE_LIBRARIES_${CMAKE_BUILD_TYPE}})
  set_property(TARGET ${TRGT} PROPERTY EXCLUDE_FROM_ALL OFF)
  set_property(TARGET ${TRGT} PROPERTY LLVMIR_SHORT_NAME ${SHORT_NAME})

  add_custom_command(OUTPUT ${FULL_OUT_LLVMIR_FILE}
    COMMAND ${LLVMIR_LINK}
    ARGS
    ${LLVMIR_ATTACH_UNPARSED_ARGUMENTS}
    -o ${FULL_OUT_LLVMIR_FILE} ${IN_FULL_LLVMIR_FILES} ${IN_OVERRIDE_OPTIONS}
    DEPENDS ${IN_FULL_LLVMIR_FILES} ${IN_FULL_LLVMIR_OVERRIDE_FILES}
    COMMENT "Linking LLVM bitcode ${OUT_LLVMIR_FILE}"
    VERBATIM)

  ## postamble
endfunction()


function(sea_link_libraries target)
  set_property(TARGET ${target} APPEND PROPERTY LLVMIR_LINK ${ARGN})
endfunction()
function(sea_overlink_libraries target)
  set_property(TARGET ${target} APPEND PROPERTY LLVMIR_OVERRIDE ${ARGN})
endfunction()

function(sea_attach_bc name)
  set(options)
  set(oneValueArgs LINKER_LANGUAGE)
  set(multiValueArgs)
  cmake_parse_arguments(ARGS
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (NOT ARGS_LINKER_LANGUAGE)
    set(ARGS_LINKER_LANGUAGE "C")
  endif()

  set(SOURCE_EXE ${name})
  set(TARGET_BC "${SOURCE_EXE}.ir")
  set(TARGET_OBJ "${SOURCE_EXE}.obj")

  # setup target with the options that we need
  set_target_properties(${SOURCE_EXE} PROPERTIES LINKER_LANGUAGE ${ARGS_LINKER_LANGUAGE})
  if(SEA_ENABLE_SMACK) # Add compile flag for smack ONLY
    target_compile_options(${SOURCE_EXE} PUBLIC -O0 -gcolumn-info -Xclang 
      -disable-O0-optnone -DMEMORY_MODEL_NO_REUSE_IMPLS)
  elseif(SEA_ENABLE_SYMBIOTIC)
    target_compile_options(${SOURCE_EXE} PUBLIC -O0 -gcolumn-info -Xclang 
      -disable-O0-optnone)
  else()
    target_compile_options(${SOURCE_EXE} PUBLIC -O1 -Xclang -disable-llvm-optzns)
  endif()
  target_compile_options(${SOURCE_EXE} PUBLIC -Wno-macro-redefined)
  target_compile_options(${SOURCE_EXE} PUBLIC -g)
  target_compile_options(${SOURCE_EXE} PUBLIC -fdeclspec)
  # do not override memcpy/memmove, we provide our own wrappers
  if (NOT SEA_LLVM_MEM_BUILTINS)
    target_compile_options(${SOURCE_EXE} PUBLIC -fno-builtin-memcpy -fno-builtin-memmove)
  endif()
  set_property(TARGET ${SOURCE_EXE} PROPERTY EXCLUDE_FROM_ALL ON)

  target_compile_definitions(${SOURCE_EXE} PRIVATE __SEAHORN__)
  if(SEA_ENABLE_CRAB)
    target_compile_definitions(${SOURCE_EXE} PRIVATE __CRAB__)
  endif()
  # when defined, check AWS_ASSERT as well as sassert
  target_compile_definitions(${SOURCE_EXE} PRIVATE __SEAHORN_ASSERT__)
  if (SEA_ENABLE_CRAB)
    target_compile_definitions(${SOURCE_EXE} PRIVATE NOCHECK)
  else()
    target_compile_definitions(${SOURCE_EXE} PRIVATE VACCHECK)
  endif()

  # convert ${SOURCE_EXE} target to llvm
  llvmir_attach_bc_target(TARGET ${TARGET_OBJ} DEPENDS ${SOURCE_EXE})
  set_property(TARGET ${TARGET_OBJ} PROPERTY EXCLUDE_FROM_ALL ON)

  get_property(IN_LLVMIR_LINK TARGET ${SOURCE_EXE} PROPERTY LLVMIR_LINK)
  get_property(IN_LLVMIR_OVERRIDE TARGET ${SOURCE_EXE} PROPERTY LLVMIR_OVERRIDE)
  set_property(TARGET ${TARGET_OBJ} PROPERTY LLVMIR_LINK ${IN_LLVMIR_LINK})
  set_property(TARGET ${TARGET_OBJ} PROPERTY LLVMIR_OVERRIDE ${IN_LLVMIR_OVERRIDE})
  # link all files of ${TARGET_OBJ} together
  # llvmir_attach_link_target(TARGET ${TARGET_BC} DEPENDS ${TARGET_OBJ})
  sea_llvm_link(${TARGET_BC} LINK ${TARGET_OBJ})
  add_dependencies(${TARGET_BC} ${TARGET_OBJ})

  set_property(TARGET ${TARGET_BC} PROPERTY EXCLUDE_FROM_ALL OFF)
endfunction()

function(sea_attach_bc_link name)
  sea_link_libraries(${name} sea_proofs.ir)
  if(SEA_ENABLE_CRAB)
    sea_link_libraries(${name} crab_proofs.ir)
  endif()
  sea_attach_bc(${name})
endfunction()

function(sea_get_file_name VAR LLVMIR_TARGET)
  get_property(DIR TARGET ${LLVMIR_TARGET} PROPERTY LLVMIR_DIR)
  get_property(FILES TARGET ${LLVMIR_TARGET} PROPERTY LLVMIR_FILES)
  set(${VAR} "${DIR}/${FILES}" PARENT_SCOPE)
endfunction()

set(VERIFY_FLAGS $ENV{VERIFY_FLAGS} CACHE STRING "Flags for verify script")
separate_arguments(VERIFY_FLAGS)

# Unit test for testing unsat
function(sea_add_unsat_test TARGET)
  sea_get_file_name(BC ${TARGET}.ir)
  add_test(NAME "${TARGET}_unsat_test" COMMAND ${VERIFY_CMD} ${VERIFY_FLAGS} --expect=unsat ${BC})
endfunction()

if(SEA_USE_LARGE_BOUNDS)
  set(MAX_BUFFER_SIZE 40)
else()
  set(MAX_BUFFER_SIZE 10)
endif()

# Compile-time definitions
add_definitions(
  -DMAX_BUFFER_SIZE=${MAX_BUFFER_SIZE}
  )

configure_file(include/config.h.in ${PROJECT_BINARY_DIR}/include/config.h)

add_subdirectory(lib)
# compilation recipes for firedancer library
add_subdirectory(stubs)
add_subdirectory(jobs)
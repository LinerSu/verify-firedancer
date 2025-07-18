add_executable(util_fd_fibre_schedule
    # ${FIREDANCER_ROOT}/src/waltz/tls/fd_tls_proto.c
    ${FIREDANCER_ROOT}/src/util/fibre/fd_fibre.c
    util_fd_fibre_schedule_harness.c
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_overlink_libraries(util_fd_fibre_schedule ucontext_override.ir)
sea_overlink_libraries(util_fd_fibre_schedule error_override.ir)
sea_attach_bc_link(util_fd_fibre_schedule)
sea_add_unsat_test(util_fd_fibre_schedule)
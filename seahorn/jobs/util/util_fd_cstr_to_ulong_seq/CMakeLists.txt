add_executable(util_fd_cstr_to_ulong_seq
    # ${FIREDANCER_ROOT}/src/waltz/tls/fd_tls_proto.c
    ${FIREDANCER_ROOT}/src/util/cstr/fd_cstr.c
    util_fd_cstr_to_ulong_seq_harness.c
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_overlink_libraries(util_fd_cstr_to_ulong_seq cstr_override.ir)
sea_attach_bc_link(util_fd_cstr_to_ulong_seq)
sea_add_unsat_test(util_fd_cstr_to_ulong_seq)
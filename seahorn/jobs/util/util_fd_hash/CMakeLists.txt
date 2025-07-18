add_executable(util_fd_hash
    # ${FIREDANCER_ROOT}/src/waltz/tls/fd_tls_proto.c
    ${FIREDANCER_ROOT}/src/util/fd_hash.c
    util_fd_hash_harness.c
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(util_fd_hash)
sea_add_unsat_test(util_fd_hash)
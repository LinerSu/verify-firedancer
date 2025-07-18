add_executable(util_fd_pod_init_delete
    # ${FIREDANCER_ROOT}/src/waltz/tls/fd_tls_proto.c
    ${FIREDANCER_ROOT}/src/util/pod/fd_pod.c
    util_fd_pod_init_delete_harness.c
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(util_fd_pod_init_delete)
sea_add_unsat_test(util_fd_pod_init_delete)
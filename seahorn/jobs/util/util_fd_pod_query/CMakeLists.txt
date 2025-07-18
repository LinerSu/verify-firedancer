add_executable(util_fd_pod_query
    # ${FIREDANCER_ROOT}/src/waltz/tls/fd_tls_proto.c
    ${FIREDANCER_ROOT}/src/util/pod/fd_pod.c
    util_fd_pod_query_harness.c
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(util_fd_pod_query)
sea_add_unsat_test(util_fd_pod_query)
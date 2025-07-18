add_executable(waltz_quic_tls_client_hs
    # ${FIREDANCER_ROOT}/src/waltz/tls/fd_tls_proto.c
    ${FIREDANCER_ROOT}/src/waltz/tls/fd_tls.c
    waltz_quic_tls_client_hs_harness.c
)
sea_link_libraries(waltz_quic_tls_client_hs fd_log_override.ir)
sea_link_libraries(waltz_quic_tls_client_hs fd_tls_helper.ir)
sea_overlink_libraries(waltz_quic_tls_client_hs fd_tls_proto_override.ir)
sea_overlink_libraries(waltz_quic_tls_client_hs fd_sha_256_override.ir)
sea_overlink_libraries(waltz_quic_tls_client_hs fd_ed25519_user_override.ir)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(waltz_quic_tls_client_hs)
sea_add_unsat_test(waltz_quic_tls_client_hs)
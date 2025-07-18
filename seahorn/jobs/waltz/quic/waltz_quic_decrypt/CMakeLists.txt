add_executable(waltz_quic_decrypt
    ${FIREDANCER_ROOT}/src/waltz/quic/crypto/fd_quic_crypto_suites.c
    waltz_quic_decrypt_harness.c
)
sea_link_libraries(waltz_quic_decrypt fd_log_override.ir)
sea_overlink_libraries(waltz_quic_decrypt fd_aes_base_ref_override.ir)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(waltz_quic_decrypt)
sea_add_unsat_test(waltz_quic_decrypt)
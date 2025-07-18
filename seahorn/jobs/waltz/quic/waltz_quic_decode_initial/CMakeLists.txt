add_executable(waltz_quic_decode_initial
    ${FIREDANCER_ROOT}/src/waltz/quic/fd_quic_proto.c
    waltz_quic_decode_initial_harness.c
)
sea_link_libraries(waltz_quic_decode_initial fd_log_override.ir)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(waltz_quic_decode_initial)
sea_add_unsat_test(waltz_quic_decode_initial)
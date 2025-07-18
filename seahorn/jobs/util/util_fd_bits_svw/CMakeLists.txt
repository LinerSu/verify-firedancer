add_executable(util_fd_bits_svw
    util_fd_bits_svw_harness.c
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(util_fd_bits_svw)
sea_add_unsat_test(util_fd_bits_svw)
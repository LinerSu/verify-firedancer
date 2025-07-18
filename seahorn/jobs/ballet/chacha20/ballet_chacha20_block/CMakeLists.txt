add_executable(ballet_chacha20_block
  ${FIREDANCER_ROOT}/src/ballet/chacha20/fd_chacha20.c
  ballet_chacha20_block_harness.c
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(ballet_chacha20_block)
sea_add_unsat_test(ballet_chacha20_block)
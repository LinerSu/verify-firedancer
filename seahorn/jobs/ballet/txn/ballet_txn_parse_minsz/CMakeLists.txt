add_executable(ballet_txn_parse_minsz
  ${FIREDANCER_ROOT}/src/ballet/txn/fd_txn_parse.c
  ballet_txn_parse_minsz_harness.c
)
configure_file(sea.yaml sea.yaml @ONLY)
sea_attach_bc_link(ballet_txn_parse_minsz)
sea_add_unsat_test(ballet_txn_parse_minsz)
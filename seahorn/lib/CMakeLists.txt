
# proof helpers
add_library(
  sea_proofs
  proof_allocators.c
  sea_allocators.c
  bcmp.c
  bounds.c
)
sea_attach_bc(sea_proofs)

# crab proof helpers
if(SEA_ENABLE_CRAB)
  add_library(crab_proofs nd_clam.c)
  sea_attach_bc(crab_proofs)
endif()
#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "fd_fseq_private.h"

int main(void) {
  ulong seq0 = nd_ulong();
  ulong footprint = fd_fseq_footprint();
  sassert(footprint == FD_FSEQ_FOOTPRINT);
  ulong size = nd_ulong();
  CRAB_ASSUME(size > 0);
  assume(size <= footprint);

  /* Allocate tcache region */
  allocate_fseq();
  ulong *array = (ulong *)initialize_bounded_seq(size, seq0);
  sassert(array);

  /* Create fseq */
  fd_fseq_shmem_t *shfseq = fd_fseq_new(hidden, seq0);
  sassert(shfseq);

  // Get the raw pointer to the fseq
  ulong *fseq = fd_fseq_join( shfseq );
  sassert(fseq);

  /* Querying fseq */
  sassert(fd_fseq_query(fseq) == seq0); // without updating it is seq0 by default
  ulong seq = nd_ulong();
  fd_fseq_update(fseq, seq);
  sassert(fd_fseq_query(fseq) == seq); // after updating it is the updated value

  // Leave the fseq region
  shfseq = (fd_fseq_shmem_t *)fd_fseq_leave(fseq);
  sassert(shfseq == hidden);

  /* Clean up */
  sassert(fd_fseq_delete(shfseq) == hidden);
  return 0;
}

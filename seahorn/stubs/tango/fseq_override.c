#include <seahorn/seahorn.h>
#include <nondet.h>
#include <sea_allocators.h>
#include <proof_allocators.h>
#include <utils.h>

#include "fd_fseq_private.h"

ulong *g_ul_ptr = NULL;
fd_fseq_shmem_t *hidden = NULL;

ulong *initialize_bounded_seq(ulong size, ulong seq0) {
  if (g_ul_ptr == NULL) {
    g_ul_ptr = (ulong *)bounded_malloc_havoc((size + 1) * sizeof(ulong));
    g_ul_ptr[0] = seq0;
  }
  return g_ul_ptr;
}

void allocate_fseq() {
  if (hidden == NULL) {
    hidden = (fd_fseq_shmem_t *)bounded_malloc_havoc(sizeof(fd_fseq_shmem_t));
  }
}

ulong *
fd_fseq_join( void * shfseq ) {

  if( FD_UNLIKELY( !shfseq ) ) {
    FD_LOG_WARNING(( "NULL shfseq" ));
    return NULL;
  }

  if( FD_UNLIKELY( !fd_ulong_is_aligned( (ulong)shfseq, fd_fseq_align() ) ) ) {
    FD_LOG_WARNING(( "misaligned shfseq" ));
    return NULL;
  }

  fd_fseq_shmem_t * fseq = (fd_fseq_shmem_t *)shfseq;

  if( FD_UNLIKELY( fseq->magic!=FD_FSEQ_MAGIC ) ) {
    FD_LOG_WARNING(( "bad magic" ));
    return NULL;
  }

  // NOTE: we do not create a flexible array, instead an external memory for the
  // sequence.
  // return &fseq->seq;
  return g_ul_ptr;
}

void *
fd_fseq_leave( ulong const * fseq ) {

  if( FD_UNLIKELY( !fseq ) ) {
    FD_LOG_WARNING(( "NULL or bad shfseq" ));
    return NULL;
  }
  // NOTE: we do not create a flexible array, instead an external memory for the
  // sequence.
  // return (void *)(fseq-2);
  return hidden;
}
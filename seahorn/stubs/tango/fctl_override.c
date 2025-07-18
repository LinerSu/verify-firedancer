#include <seahorn/seahorn.h>
#include <nondet.h>
#include <sea_allocators.h>
#include <proof_allocators.h>
#include <utils.h>

#include "fd_fctl.h"

void *
fd_fctl_new( void * shmem,
             ulong  rx_max ) {
  if( FD_UNLIKELY( !shmem ) ) {
    FD_LOG_WARNING(( "NULL shmem" ));
    return NULL;
  }

  if( FD_UNLIKELY( !fd_ulong_is_aligned( (ulong)shmem, fd_fctl_align() ) ) ) {
    FD_LOG_WARNING(( "misaligned shmem" ));
    return NULL;
  }

  if( FD_UNLIKELY( rx_max>FD_FCTL_RX_MAX_MAX ) ) {
    FD_LOG_WARNING(( "too large rx_max" ));
    return NULL;
  }

  fd_fctl_t * fctl = (fd_fctl_t *)shmem;

  fctl->rx_max    = (ushort)rx_max;
  fctl->rx_cnt    = (ushort)0;
  fctl->in_refill = 0;
  fctl->cr_burst  = 0UL;
  fctl->cr_max    = 0UL;
  fctl->cr_resume = 0UL;
  fctl->cr_refill = 0UL;
#ifdef __SEAHORN__
  fctl->rx = bounded_malloc_havoc(sizeof(fd_fctl_private_rx_t) * rx_max); // Explicitly set the rx pointer
#endif

  return shmem;
}

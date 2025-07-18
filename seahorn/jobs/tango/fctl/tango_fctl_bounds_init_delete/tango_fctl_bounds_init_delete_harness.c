#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <stdalign.h>
#include <utils.h>

#include "fd_fctl.h"

#define SEA_FD_FCTL_RX_MAX 10UL

int main() {
  fd_fctl_t fctl;
  ulong rx_size = nd_ulong();
  CRAB_ASSUME(rx_size > 0UL);
  assume(rx_size <= SEA_FD_FCTL_RX_MAX);
  memhavoc(&fctl, fd_fctl_footprint(rx_size));
  void *rmem = fd_fctl_new(&fctl, rx_size);
  if (!rmem) {
    return 0;
  }
  fd_fctl_t *f = fd_fctl_join(rmem);

  sassert(fd_fctl_rx_max(f) == rx_size);
  sassert(fd_fctl_rx_cnt(f) == 0);
  sassert(fd_fctl_cr_burst(f) == 0);
  sassert(fd_fctl_cr_max(f) == 0);
  sassert(fd_fctl_cr_resume(f) == 0);
  sassert(fd_fctl_cr_refill(f) == 0);

  ulong idx = nd_ulong();
  assume(idx < rx_size);
  sassert(sea_is_dereferenceable(&fd_fctl_private_rx(f)[idx],
                                 sizeof(fd_fctl_private_rx_t)));

  fd_fctl_t *f2 = fd_fctl_leave(fd_fctl_delete(f));
  sassert(f2 == &fctl);
  return 0;
}
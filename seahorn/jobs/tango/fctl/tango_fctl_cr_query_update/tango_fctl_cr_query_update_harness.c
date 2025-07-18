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
  ulong cnt = nd_ulong();
  CRAB_ASSUME(rx_size > 0UL);
  assume(rx_size <= SEA_FD_FCTL_RX_MAX);
  assume(cnt <= rx_size);
  memhavoc(&fctl, fd_fctl_footprint(rx_size));
  void *rmem = fd_fctl_new(&fctl, rx_size);
  if (!rmem) {
    return 0;
  }
  fd_fctl_t *f = fd_fctl_join(rmem);

  ulong *rx_seq = bounded_malloc_havoc(sizeof(ulong) * rx_size);
  ulong *rx_slow = bounded_malloc_havoc(sizeof(ulong) * rx_size);

  for (ulong i = 0; i < cnt; i++) {
    ulong old_cnt = fd_fctl_rx_cnt(f);
    ulong rx_cr_max = nd_ulong();
    assume(rx_cr_max > 0UL);
    assume(rx_cr_max <= (ulong)LONG_MAX);
    if (fd_fctl_cfg_rx_add(f, rx_cr_max, &rx_seq[i], &rx_slow[i])) {
      sassert(old_cnt + 1 == fd_fctl_rx_cnt(f));
    };
  }
  sassert(cnt >= fd_fctl_rx_cnt(f));

  

  fd_fctl_t *f2 = fd_fctl_leave(fd_fctl_delete(f));
  sassert(f2 == &fctl);
  return 0;
}
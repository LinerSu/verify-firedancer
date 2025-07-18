#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "fd_dcache_private.h"

#define ALIGN_ROUND_UP(value, alignment)                                       \
  (((value) + ((alignment) - 1)) & ~((alignment) - 1))

#define MAX_DATA_SZ 1024UL
#define MAX_APP_SZ 1024UL
fd_dcache_private_hdr_t const *glb_dcahe_const_ptr = NULL;
fd_dcache_private_hdr_t *glb_dcahe_ptr = NULL;

ulong fd_dcache_footprint(ulong data_sz, ulong app_sz) {
  assume(data_sz <= MAX_DATA_SZ);
  assume(app_sz <= MAX_APP_SZ);
#ifdef __CRAB__
  CRAB_ASSUME(data_sz > 0);
  CRAB_ASSUME(app_sz > 0);
  ulong cnt = nd_ulong();
  ulong min_cnt = sizeof(fd_dcache_private_hdr_t) + 1 + 1;
  CRAB_ASSUME(cnt >= ALIGN_ROUND_UP(min_cnt, FD_DCACHE_ALIGN));
  ulong max_cnt = sizeof(fd_dcache_private_hdr_t) + MAX_DATA_SZ + MAX_APP_SZ;
  // numerical domain for unbound integer lacks approximate aligning values
  // here, however, we know the bounds of results will be, so
  CRAB_ASSUME(cnt <= ALIGN_ROUND_UP(max_cnt, FD_DCACHE_ALIGN));
#else
  ulong cnt = sizeof(fd_dcache_private_hdr_t) + data_sz + app_sz;
  cnt = ALIGN_ROUND_UP(cnt, FD_DCACHE_ALIGN);
#endif
  return cnt;
}

uchar *fd_dcache_app_laddr(uchar *dcache) {
  fd_dcache_private_hdr_t *hdr = fd_dcache_private_hdr(dcache);
  return hdr->app;
}
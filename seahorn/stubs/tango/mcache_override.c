#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "fd_mcache_private.h"

#define ALIGN_ROUND_UP(value, alignment)                                       \
  (((value) + ((alignment) - 1)) & ~((alignment) - 1))

fd_mcache_private_hdr_t const *glb_mcahe_const_ptr = NULL;
fd_mcache_private_hdr_t *glb_mcahe_ptr = NULL;

ulong fd_mcache_footprint(ulong depth, ulong app_sz) {
  assume(depth <= MAX_DEPTH);
  assume(app_sz <= MAX_APP_SZ);
#ifdef __CRAB__
  CRAB_ASSUME(depth > 0);
  CRAB_ASSUME(app_sz > 0);
  ulong cnt = nd_ulong();
  ulong min_cnt = sizeof(fd_mcache_private_hdr_t) + 1 + 1;
  CRAB_ASSUME(cnt >= ALIGN_ROUND_UP(min_cnt, FD_MCACHE_ALIGN));
  ulong max_cnt = sizeof(fd_mcache_private_hdr_t) + MAX_DEPTH + MAX_APP_SZ;
  // numerical domain for unbound integer lacks approximate aligning values
  // here, however, we know the bounds of results will be, so
  CRAB_ASSUME(cnt <= ALIGN_ROUND_UP(max_cnt, FD_MCACHE_ALIGN));
#else
  ulong cnt = sizeof(fd_mcache_private_hdr_t) + depth + app_sz;
  cnt = ALIGN_ROUND_UP(cnt, FD_MCACHE_ALIGN);
#endif
  return cnt;
}

uchar *fd_mcache_app_laddr( fd_frag_meta_t * mcache ) {
  fd_mcache_private_hdr_t * hdr = fd_mcache_private_hdr( mcache );
  return hdr->data;
}
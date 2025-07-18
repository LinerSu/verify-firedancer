#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "fd_mcache_private.h"

#define ALIGN_ROUND_UP(value, alignment)                                       \
  (((value) + ((alignment) - 1)) & ~((alignment) - 1))

int main(void) {

  ulong depth = nd_ulong();
  assume(depth <= MAX_DEPTH);
  ulong app_sz = nd_ulong();
  assume(app_sz <= MAX_APP_SZ);
  ulong footprint = fd_mcache_footprint(depth, app_sz);
  if (footprint == 0 || footprint > MEM_BLOCK * 2) {
    return 0;
  }

  /* Allocate mcache region */
  fd_mcache_private_hdr_t hdr;
  memhavoc(&hdr, sizeof(fd_mcache_private_hdr_t));
  hdr.meta = bounded_malloc_havoc(depth * sizeof(fd_frag_meta_t));
  hdr.data = bounded_malloc_havoc(app_sz * sizeof(uchar));
  hdr.seq = bounded_malloc_havoc(FD_MCACHE_SEQ_CNT * sizeof(ulong));

  /* Create mcache */
  ulong seq0 = nd_ulong();
  assume(seq0 < depth);
  void *cache = fd_mcache_new(&hdr, depth, app_sz, seq0);
  if (!cache) {
    return 0;
  }
  fd_frag_meta_t *mcache = fd_mcache_join(cache);

  /* Verify seq region */

  sassert(fd_mcache_seq0(mcache) == seq0);
  sassert(fd_mcache_depth(mcache) == depth);
  ulong *seq = fd_mcache_seq_laddr(mcache);
  sassert(sea_is_dereferenceable(seq, FD_MCACHE_SEQ_CNT * sizeof(ulong)));

  /* Verify app region */

  sassert(fd_mcache_app_sz(mcache) == app_sz);

  uchar *app = fd_mcache_app_laddr(mcache);
  sassert(sea_is_dereferenceable(app, app_sz));

  /* Clean up */

  fd_mcache_delete(fd_mcache_leave(mcache));
  return 0;
}

#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "fd_dcache_private.h"

int main(void) {
  ulong data_sz = nd_ulong();
  ulong app_sz = nd_ulong();
  ulong footprint = fd_dcache_footprint(data_sz, app_sz);
  if (footprint == 0 || footprint > MEM_BLOCK * 2) {
    return 0;
  }
  fd_dcache_private_hdr_t hdr;

  /* Create dcache */
  void *cache = fd_dcache_new(&hdr, data_sz, app_sz);
  if (!cache) {
    return 0;
  }
  /* Allocate dcache region */
  hdr.data = bounded_malloc_havoc(data_sz * sizeof(uchar));
  hdr.app = bounded_malloc_havoc(app_sz * sizeof(uchar));
  uchar *dcache = fd_dcache_join(cache);

  // check if dcache can be compacted
  ulong mtu = nd_ulong();
  ulong depth = nd_ulong();
  assume(mtu <= 256UL);
  CRAB_ASSUME(mtu > 0UL);
  assume(depth <= 2UL);
  CRAB_ASSUME(depth > 0UL);
  uchar *base = bounded_malloc_havoc(mtu * sizeof(uchar));
  if (fd_dcache_compact_is_safe(base, dcache, mtu, depth)) {
    // Get the chunk indices and watermark
    ulong chunk0 = fd_dcache_compact_chunk0(base, dcache);
    ulong chunk1 = fd_dcache_compact_chunk1(base, dcache);
    ulong wmark = fd_dcache_compact_wmark(base, dcache, mtu);
    ulong ret = fd_dcache_compact_next(chunk0, mtu, chunk0, wmark);
    sassert(ret >= chunk0 && ret <= wmark);
  }

  /* Clean up dcache */
  fd_dcache_delete(fd_dcache_leave(dcache));

  return 0;
}
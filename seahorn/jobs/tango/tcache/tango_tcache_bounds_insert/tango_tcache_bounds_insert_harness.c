#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "fd_tcache.h"

int main(void) {
  ulong depth = nd_ulong();
  ulong map_cnt = nd_ulong();
  ulong footprint = fd_tcache_footprint(depth, map_cnt);
  if (footprint == 0 || footprint > MEM_BLOCK * 2) {
    return 0;
  }

  /* Allocate tcache region */
  fd_tcache_t tcache_mem;
  tcache_mem.ring = bounded_malloc_havoc(depth * sizeof(ulong));
  tcache_mem.map = bounded_malloc_havoc(map_cnt * sizeof(ulong));
  // Q: why fd_tcache_ring_laddr cause collapse?
  // accessing items from ring or map will cause dsa node collapse to a sequence

  /* Create tcache */
  void *cache = fd_tcache_new(&tcache_mem, depth, map_cnt);
  if (!cache) {
    return 0;
  }
  fd_tcache_t *tcache = fd_tcache_join(cache);

  /* Querying tcache */
  int dup = 0;
  ulong depth_idx = nd_ulong();
  assume(depth_idx < depth);
  ulong tag = fd_ulong_hash( depth_idx + 1UL );
  sassert(!fd_tcache_tag_is_null(tag));

  ulong * _oldest = fd_tcache_oldest_laddr( tcache );
  ulong   oldest  = _oldest[0];
  // tag as non FD_TCACHE_TAG_NULL
  // macros perform textual substitution
  FD_TCACHE_INSERT(dup, oldest, tcache->ring, tcache->depth, tcache->map,
                   tcache->map_cnt, tag);
  sassert(oldest < tcache->depth);

  /* Clean up */
  fd_tcache_delete(fd_tcache_leave(tcache));

  return 0;
}

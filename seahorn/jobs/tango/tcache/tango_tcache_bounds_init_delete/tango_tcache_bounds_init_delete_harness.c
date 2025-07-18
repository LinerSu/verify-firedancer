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

  /* Verify ring region */

  sassert(fd_tcache_depth(tcache) == depth);
  ulong *ring = fd_tcache_ring_laddr(tcache);
  ulong idx = nd_ulong();
  assume(idx <= depth);
  sassert(sea_is_dereferenceable(ring, idx * sizeof(ulong)));

  /* Verify map region */
  sassert(fd_tcache_map_cnt(tcache) == map_cnt);
  ulong *map = fd_tcache_map_laddr(tcache);
  ulong idx2 = nd_ulong();
  assume(idx2 <= map_cnt);
  sassert(sea_is_dereferenceable(map, idx2 * sizeof(ulong)));

  /* Clean up */
  fd_tcache_delete(fd_tcache_leave(tcache));

  return 0;
}

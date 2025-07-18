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

  /* Verify data region */
  sassert(fd_dcache_data_sz(dcache) == data_sz);
  uchar *data = dcache;
  ulong idx = nd_ulong();
  assume(idx <= data_sz);
  sassert(sea_is_dereferenceable(data, idx * sizeof(uchar)));

  /* Verify app region */
  sassert(fd_dcache_app_sz(dcache) == app_sz);
  uchar *app = fd_dcache_app_laddr(dcache);
  ulong idx2 = nd_ulong();
  assume(idx2 <= app_sz);
  sassert(sea_is_dereferenceable(app, idx2 * sizeof(uchar)));

  /* Clean up dcache */
  fd_dcache_delete(fd_dcache_leave(dcache));

  return 0;
}

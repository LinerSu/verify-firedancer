#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "util/fd_util_base.h"


int main(void) {
  ulong sz = nd_ulong();
  ulong seed = nd_ulong();
  ulong seed2 = nd_ulong();

  assume(sz < 10UL);

  ulong *buf = (ulong *)bounded_malloc_havoc(sz * sizeof(ulong));
  ulong *buf2 = (ulong *)bounded_malloc_havoc(sz * sizeof(ulong));

  ulong hash = fd_hash(seed, buf, sz * sizeof(ulong));
  ulong hash2 = fd_hash(seed2, buf, sz * sizeof(ulong));
  sassert(buf);
  // sassert(hash != hash2);
  ulong hash3 = fd_hash_memcpy(seed, buf, buf, sz * sizeof(ulong));
  // sassert(hash == hash3);
  sassert(buf2);
  return 0;
}

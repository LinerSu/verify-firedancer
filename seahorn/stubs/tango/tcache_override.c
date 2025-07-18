#include <seahorn/seahorn.h>
#include <nondet.h>
#include <sea_allocators.h>
#include <proof_allocators.h>
#include <utils.h>

#include "fd_tcache.h"

#define ALIGN_ROUND_UP(value, alignment)                                       \
  (((value) + ((alignment)-1)) & ~((alignment)-1))

#define MAX_DEPTH 10
#define MAX_MAP_CNT 10

ulong fd_tcache_footprint(ulong depth, ulong map_cnt) {
  assume(depth <= MAX_DEPTH);
  assume(map_cnt <= MAX_MAP_CNT);
#ifdef __CRAB__
  CRAB_ASSUME(depth > 0);
  CRAB_ASSUME(map_cnt > 0);
  ulong cnt = nd_ulong();
  ulong min_cnt = sizeof(fd_tcache_t) + 1 + 1;
  min_cnt *= sizeof(ulong);
  CRAB_ASSUME(cnt >= ALIGN_ROUND_UP(min_cnt, FD_TCACHE_ALIGN));
  ulong max_cnt = sizeof(fd_tcache_t) + MAX_DEPTH + MAX_MAP_CNT;
  max_cnt *= sizeof(ulong);
  // numerical domain for unbound integer lacks approximate aligning values
  // here, however, we know the bounds of results will be, so
  CRAB_ASSUME(cnt <= ALIGN_ROUND_UP(max_cnt, FD_TCACHE_ALIGN));
#else
  ulong cnt = sizeof(fd_tcache_t) + depth + map_cnt;
  cnt *= sizeof(ulong);
  cnt = ALIGN_ROUND_UP(cnt, FD_TCACHE_ALIGN);
#endif
  return cnt;
}
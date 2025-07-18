#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "util/pod/fd_pod.h"

#define FD_POD_FOOTPRINT_MAX 16UL
#define FD_POD_SEA_MIN 8UL

int main(void) {
  // Choose a pod max between FD_POD_SEA_MIN and FD_POD_FOOTPRINT_MAX.
  ulong max = nd_ulong();
  assume(FD_POD_SEA_MIN <= max && max <= FD_POD_FOOTPRINT_MAX);
  ulong max2 = nd_ulong();
  assume(FD_POD_SEA_MIN <= max2 && max2 <= FD_POD_FOOTPRINT_MAX);

  // Create a memory buffer large enough for a pod.
  uchar *mem = bounded_malloc_havoc(max * sizeof(ulong));
  if (!mem) {
    // If memory allocation fails, we cannot proceed.
    return 0;
  }

  // Construct the pod using fd_pod_new. It should return the input pointer.
  void *shpod = fd_pod_new(mem, max);
  if (!shpod) {
    // If pod creation fails, we cannot proceed.
    return 0;
  }
  sassert(shpod == (void *)mem);

  // Join the pod from shared memory.
  uchar *pod = fd_pod_join(shpod);
  sassert(pod == mem);

  // check if compact is applicable and resize back
  // ulong r = nd_ulong();
  // int   type    = (int)(r & 15U);         r >>= 4;
  // ulong path_sz = 1UL + (ulong)(r & 63U); r >>= 6;
  // int   remove  = (int)(r & 1U);          r >>= 1;
  // int   compact = !(r & 15U);             r >>= 4;
  // int   full    = (int)(r & 1U);          r >>= 1;
  // if (compact) {
  //   ulong orig_max = fd_pod_max(pod);
  //   ulong orig_cnt = fd_pod_cnt(pod);

  //   ulong compact_max = fd_pod_compact(pod, full);
  //   sassert(fd_pod_max(pod) == compact_max);

  //   if (!full)
  //     sassert(fd_pod_max(pod) == orig_max);
  //   sassert(fd_pod_cnt(pod) == orig_cnt);

  //   sassert(fd_pod_resize(pod, max) == max);

  //   sassert(fd_pod_max(pod) == max);
  //   sassert(fd_pod_cnt(pod) == orig_cnt);
  // }
  sassert(fd_pod_resize(pod, max2) == max2);
  // Call leave on the pod.
  void *left = fd_pod_leave(pod);
  sassert(left == (void *)pod);

  // Delete the pod (which in our implementation is a no-op).
  void *del = fd_pod_delete(shpod);
  sassert(del == shpod);

  return 0;
}
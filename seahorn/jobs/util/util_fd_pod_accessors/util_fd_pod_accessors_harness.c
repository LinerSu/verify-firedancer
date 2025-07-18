#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "util/pod/fd_pod.h"

#define FD_POD_FOOTPRINT_MAX 128UL
#define FD_POD_SEA_MIN 64UL

int main(void) {
  // Choose a pod max between FD_POD_SEA_MIN and FD_POD_FOOTPRINT_MAX.
  ulong max = nd_ulong();
  assume(FD_POD_SEA_MIN <= max && max <= FD_POD_FOOTPRINT_MAX);

  // Create a memory buffer large enough for a pod.
  uchar *mem = bounded_malloc_havoc(max);
  if (!mem) {
    // If memory allocation fails, we cannot proceed.
    return 0;
  }

  // Construct the pod using fd_pod_new. It should return the input pointer.
  void *shpod = fd_pod_new(mem, max);
  sassert(shpod == (void *)mem);

  // Join the pod from shared memory.
  uchar *pod = fd_pod_join(shpod);
  sassert(pod == mem);

  // Call few accessors on the pod.
  ulong hdr_sz = 3UL*fd_ulong_svw_enc_sz( max );
  sassert(fd_pod_max(pod) == max);
  sassert(fd_pod_used(pod) == hdr_sz);
  sassert(fd_pod_cnt(pod) == 0UL);
  sassert(fd_pod_avail(pod) == max - hdr_sz);
  // Call leave on the pod.
  void *left = fd_pod_leave(pod);
  sassert(left == (void *)pod);

  // Delete the pod (which in our implementation is a no-op).
  void *del = fd_pod_delete(shpod);
  sassert(del == shpod);

  return 0;
}
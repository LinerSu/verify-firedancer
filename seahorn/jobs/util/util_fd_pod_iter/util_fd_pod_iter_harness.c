#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "util/pod/fd_pod.h"

#define FD_POD_FOOTPRINT_MAX 128UL
#define FD_POD_SEA_MIN 64UL

/*
The iterator keeps track the current position in the pod and where to stop.
uchar const * cursor points to the current key-val pair
uchar const * stop points to the stop of the current key-val pair.
The iterator starts at the first key-val pair in the pod.

pod info C struct stores decoded information about a key-val pair.
*/

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

  // Iterate over the pod to check its contents.
  ulong idx = 0UL;
  fd_pod_iter_t iter;
  memhavoc(&iter, sizeof(fd_pod_iter_t));
  fd_pod_iter_init(pod, &iter);
  for (;!fd_pod_iter_done(&iter); fd_pod_iter_next(&iter)) {
    fd_pod_info_t info = fd_pod_iter_info(iter);
    sassert(!info.parent);
    idx++;
  }
  sassert(idx > 0UL);
  // Call leave on the pod.
  void *left = fd_pod_leave(pod);
  sassert(left == (void *)pod);

  // Delete the pod (which in our implementation is a no-op).
  void *del = fd_pod_delete(shpod);
  sassert(del == shpod);

  return 0;
}
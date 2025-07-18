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
  if (!shpod) {
    // If pod creation fails, we cannot proceed.
    return 0;
  }
  sassert(shpod == (void *)mem);

  // Join the pod from shared memory.
  uchar *pod = fd_pod_join(shpod);
  sassert(pod == mem);

  // Query an item in the pod
  ulong cnt = fd_pod_cnt( pod );
  if (cnt <= 64UL) {
    fd_pod_info_t info[64];
    memhavoc(info, 64 * sizeof(fd_pod_info_t));
    fd_pod_list(pod, info);
    ulong idx = nd_ulong();
    assume(idx < cnt);
    fd_pod_info_t query[1];
    memhavoc(query, sizeof(fd_pod_info_t));
    if (!fd_pod_query( pod, info[idx].key, query )) {
      // Check the queried information matches the info list
      sassert(info[idx].key_sz == query->key_sz);
      sassert(info[idx].key == query->key);
      sassert(info[idx].val_type == query->val_type);
      sassert(info[idx].val_sz == query->val_sz);
      sassert(info[idx].val == query->val);
      sassert(info[idx].parent == query->parent);
    }
  }

  // Call leave on the pod.
  void *left = fd_pod_leave(pod);
  sassert(left == (void *)pod);

  // Delete the pod (which in our implementation is a no-op).
  void *del = fd_pod_delete(shpod);
  sassert(del == shpod);

  return 0;
}
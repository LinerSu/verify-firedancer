#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "util/pod/fd_pod.h"
#define FD_POD_FOOTPRINT_MAX 128UL
#define FD_POD_SEA_MIN 64UL

/*
pod memory layout:
Header: 
three fields to indicate maximum number of bytes, used bytes, and count of key-value pairs.

max will be the one we control
value of max is encoded based on number of bytes can be represented in the pod.
E.g. if max is 128, then it takes 2 bytes to encode based on fd_ulong_svw_enc_sz.
suppose sz = fd_ulong_svw_enc_sz(max);

used bytes after init is the number of bytes encoded for max, used, and count.
so its value is 3 * sz.

count initially is 0.

sz = fd_ulong_svw_enc_sz(max);
          +------------+------------+------------+
Field     | max bytes  | used bytes | count used |
Size      |   (sz B)   |   (sz B)   |   (sz B)   |
          +------------+------------+------------+
cnt <= used <= max always.

After the header, it stores number of count key-value pairs.
Each has this structure:
sz = fd_ulong_svw_enc_sz(key);
the key is short, so the maximum size of key is 9 bytes.
key takes strlen(key)+1 bytes, so key_sz = strlen(key)+1;
          +----------+--------------+------------+----------+--------------+
Field     |  key_sz  |      key     |  val type  |  val_sz  |      val     |
Size      |  (1 B)   |  (key_sz B)  |   (1 B)    |  (1 B)   |  (val_sz B)  |
          +----------+--------------+------------+----------+--------------+
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

  // Call leave on the pod.
  void *left = fd_pod_leave(pod);
  sassert(left == (void *)pod);

  // Delete the pod (which in our implementation is a no-op).
  void *del = fd_pod_delete(shpod);
  sassert(del == shpod);

  return 0;
}
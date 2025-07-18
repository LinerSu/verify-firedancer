#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "waltz/quic/fd_quic_proto.h"
#include "waltz/quic/fd_quic_proto.c"

int main(void) {
  size_t size = nd_size_t();
  assume(size >= sizeof(fd_quic_initial_t));
  uchar *buf = bounded_malloc_havoc(size);

  fd_quic_initial_t initial;

  ulong byte_cnt = fd_quic_decode_initial(&initial, buf, size);
  if (byte_cnt == FD_QUIC_PARSE_FAIL)
    return 0;

  sassert(byte_cnt <= size);

  sassert(initial.dst_conn_id_len <= 20);
  sassert(initial.src_conn_id_len <= 20);
  sassert(initial.len < (1UL << 62));
  return 0;
}

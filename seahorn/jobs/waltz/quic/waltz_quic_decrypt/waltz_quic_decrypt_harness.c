#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "waltz/quic/fd_quic.h"
#include "waltz/quic/crypto/fd_quic_crypto_suites.h"

#define MTU (2048UL)

int main(void) {
  ulong buf_sz = nd_ulong();
  assume(buf_sz > 0UL); assume(buf_sz <= MTU);
  uchar *buf = bounded_malloc_havoc(buf_sz);

  ulong pkt_number_off = nd_ulong();
  assume(pkt_number_off < buf_sz);

  fd_quic_crypto_keys_t keys = {0};

  int result =
      fd_quic_crypto_decrypt_hdr(buf, buf_sz, pkt_number_off, &keys);
  sassert(result == FD_QUIC_SUCCESS || result == FD_QUIC_FAILED);
  if (result != FD_QUIC_SUCCESS)
    return 0;
  ulong pkt_number = nd_ulong();

  int result2 = fd_quic_crypto_decrypt(buf, buf_sz, pkt_number_off, pkt_number, &keys);
  sassert(result2 == FD_QUIC_SUCCESS || result2 == FD_QUIC_FAILED);
  if (result2 != FD_QUIC_SUCCESS)
    return 0;
  sassert(buf);
  return 0;
}

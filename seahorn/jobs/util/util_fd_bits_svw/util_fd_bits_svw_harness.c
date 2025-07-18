#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "util/bits/fd_bits.h"

int main(void) {
  ulong x = nd_ulong();
  ulong sz = fd_ulong_svw_enc_sz(x);
  sassert(sz <= FD_ULONG_SVW_ENC_MAX);

  uchar *b0 = (uchar *)bounded_malloc_havoc(sz);
  uchar *b0_ = fd_ulong_svw_enc(b0, x);
  sassert(b0 + sz == b0_);

  uchar *b1 = (uchar *)bounded_malloc_havoc(sz);
  uchar *b1_ = fd_ulong_svw_enc_fixed(b1, sz, x);
  sassert(b1 + sz == b1_);

  sassert(0 == memcmp(b0, b1, sz));

  ulong sz_ = fd_ulong_svw_dec_sz(b0);
  sassert(sz == sz_);
  ulong x0 = fd_ulong_svw_dec_fixed(b0, sz);
  sassert(x == x0);
  return 0;
}

#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <stdalign.h>
#include <utils.h>

#include "ballet/chacha20/fd_chacha20.h"

/*
the ChaCha20 stream cipher

A 256-bit key (32 bytes)

A 32-bit block counter (a single 4-byte integer)

A 96- or 64-bit nonce/IV (12 or 8 bytes, depending on the variant)
*/

int main() {
  /* Create a ChaCha20 block */
  uint *block1 = bounded_malloc_havoc(FD_CHACHA20_BLOCK_SZ);
  uint *block2 = bounded_malloc_havoc(FD_CHACHA20_BLOCK_SZ);
  uint *key = bounded_malloc_havoc(FD_CHACHA20_KEY_SZ);
  uint *idx_nonce = bounded_malloc_havoc(16UL);
  int idx = nd_int();

  assume(idx >= 0 && idx < 16);
  assume(block1[idx] != block2[idx]);

  fd_chacha20_block(block1, key, idx_nonce);
  fd_chacha20_block(block2, key, idx_nonce);

  sassert(block1[idx] != block2[idx]);
  return 0;
}
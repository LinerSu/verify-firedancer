#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <stdalign.h>
#include <utils.h>

#include "fd_chacha20rng.h"

int main() {
  fd_chacha20rng_t rng;
  memhavoc(&rng, fd_chacha20rng_footprint());
  int mod = nd_int();

  assume(mod == FD_CHACHA20RNG_MODE_MOD || mod == FD_CHACHA20RNG_MODE_SHIFT);
  fd_chacha20rng_t *r = fd_chacha20rng_join(fd_chacha20rng_new(&rng, mod));
  rng.key = bounded_malloc_havoc(32UL);
  rng.buf = bounded_malloc_havoc(FD_CHACHA20RNG_BUFSZ);

  uchar *key = bounded_malloc_havoc(32UL);
  r = fd_chacha20rng_init(r, key);
  sassert(r);
  sassert(r->buf_off == r->buf_fill);
  sassert(r->buf_off == 0UL);

  r = fd_chacha20rng_delete(fd_chacha20rng_leave(r));
  sassert(r == &rng);
  return 0;
}
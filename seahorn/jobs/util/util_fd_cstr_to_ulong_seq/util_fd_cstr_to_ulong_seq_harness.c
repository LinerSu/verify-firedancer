#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>
#include <bounds.h>

#include "util/cstr/fd_cstr.h"

int main(void) {
  /* Create a nondeterministic buffer of size BUF_SIZE and force it to be a
   * valid C string */
  uint len = nd_uint();
  assume(len < sea_max_buffer_size());
  CRAB_ASSUME(len > 0); // Ensure non-empty string
  char *buf = bounded_malloc_havoc(len);
  buf[len] = '\0';

  /* Prepare a sequence array with a bounded size */
  ulong seq_sz = nd_ulong();
  assume(seq_sz < 20UL);
  CRAB_ASSUME(seq_sz > 0UL); // Ensure non-empty sequence
  ulong * seq = bounded_malloc_havoc(seq_sz * sizeof(ulong));

  /* Call the function under verification */
  ulong ret = fd_cstr_to_ulong_seq(buf, seq, seq_sz);

  /* Check: if the conversion succeeded (non-zero ret),
      then for each parsed element (up to the provided buf capacity),
      the sequence values should be monotonic non-decreasing. */
  for (unsigned int i = 1; i < seq_sz && i < ret; i++) {
    sassert(seq[i] >= seq[i - 1]);
  }

  /* Also check that ret is within a reasonable bound.
      (Since a malformed string returns 0, ret==0 is acceptable.) */
  sassert(ret <= 20UL);

  return 0;
}
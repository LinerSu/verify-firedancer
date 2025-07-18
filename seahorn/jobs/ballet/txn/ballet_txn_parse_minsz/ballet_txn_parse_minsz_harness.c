#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <stdalign.h>
#include <utils.h>

#include "ballet/txn/fd_txn.h"

#define ALIGN_UP(x, align) (((x) + (align) - 1) & ~((align) - 1))
#define TXN_BUF_TOTAL_SIZE ALIGN_UP(FD_TXN_MAX_SZ, alignof(fd_txn_t))

/* Prove that there is no valid serialized txn with size less than
   FD_TXN_MIN_SERIALIZED_SZ. */

int main() {
  /* Input */
  ulong input_sz = nd_ulong();
  assume( input_sz<FD_TXN_MIN_SERIALIZED_SZ );
  uchar * input = bounded_malloc_havoc( input_sz );
  if( !input ) return 0;

  /* Parsing target buffers */
  fd_txn_parse_counters_t counters = {0};
  uchar *txn_buf = (uchar *)bounded_malloc_havoc(TXN_BUF_TOTAL_SIZE);
  if (!txn_buf)
    return 0;

  /* Parse */
  ulong res = fd_txn_parse( input, input_sz, txn_buf, &counters );

  /* Parsing must have failed */
  sassert( res==0UL );
  return 0;
}

#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <stdalign.h>
#include <utils.h>

#include "ballet/txn/fd_txn.h"

#define ALIGN_UP(x, align) (((x) + (align) - 1) & ~((align) - 1))
#define TXN_BUF_TOTAL_SIZE ALIGN_UP(FD_TXN_MAX_SZ, alignof(fd_txn_t))

/* This proof proves that the signed payload of a
   transaction (Message) cannot overlap with that of a gossip message
   (CrdsData). */

int main() {
  /* CrdsData (the to-be-signed payload of a gossip message) starts
     with a 32-bit little-endian type ID. */
  uchar prefix[4];
  memhavoc(prefix, 4UL);
  assume(prefix[0] < 0x20);
  assume(prefix[1] == 0x00);
  assume(prefix[2] == 0x00);
  assume(prefix[3] == 0x00);

  /* Craft a transaction (with fake signatures)
     (Firedancer currently can't parse a transaction without the
      signatures) */
  uchar *txn = bounded_malloc_havoc(FD_TXN_MTU);
  if (!txn)
    return 0;
  if (prefix[0] * 64 > FD_TXN_MTU - 1)
    return 0;
  txn[0] = prefix[0]; /* signature count */

  /* Copy prefix to message part */
  ulong msg_off = 1 + txn[0] * 64;
  txn[msg_off + 0] = prefix[0];
  txn[msg_off + 1] = prefix[1];
  txn[msg_off + 2] = prefix[2];
  txn[msg_off + 3] = prefix[3];

  /* Partially constrained transaction size */
  ulong txn_sz = nd_ulong();
  assume(txn_sz > msg_off + 4);
  assume(txn_sz <= FD_TXN_MTU);

  /* Parsing target buffers */
  fd_txn_parse_counters_t counters = {0};
  uchar *txn_buf = (uchar *)bounded_malloc_havoc(TXN_BUF_TOTAL_SIZE);
  if (!txn_buf)
    return 0;

  /* Parsing must fail */
  ulong res = fd_txn_parse(txn, txn_sz, txn_buf, &counters);
  sassert(res == 0UL);
  return 0;
}

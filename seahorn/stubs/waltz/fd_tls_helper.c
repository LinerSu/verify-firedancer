#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "stubs/fd_tls_helper.h"

extern void *tls_rand_ctx;
extern void *fd_tls_sign_ctx;

void *tls_rand(void *ctx, void *buf, ulong bufsz) {
  sassert(tls_rand_ctx == ctx);
  sassert(sea_is_dereferenceable(buf, bufsz));
  memhavoc(buf, bufsz);
  return buf;
}

void tls_secrets(void const *handshake, void const *recv_secret,
                        void const *send_secret, uint encryption_level) {
  sassert(sea_is_dereferenceable(handshake, sizeof(fd_tls_estate_base_t)));
  sassert(sea_is_dereferenceable(recv_secret, 32UL));
  sassert(sea_is_dereferenceable(send_secret, 32UL));
  sassert((encryption_level == FD_TLS_LEVEL_INITIAL) |
          (encryption_level == FD_TLS_LEVEL_EARLY) |
          (encryption_level == FD_TLS_LEVEL_HANDSHAKE) |
          (encryption_level == FD_TLS_LEVEL_APPLICATION));
}

int tls_sendmsg(void const *handshake, void const *record,
                       ulong record_sz, uint encryption_level, int flush) {
  sassert(sea_is_dereferenceable(handshake, sizeof(fd_tls_estate_base_t)));
  sassert(sea_is_dereferenceable(record, record_sz));
  sassert((encryption_level == FD_TLS_LEVEL_INITIAL) |
          (encryption_level == FD_TLS_LEVEL_EARLY) |
          (encryption_level == FD_TLS_LEVEL_HANDSHAKE) |
          (encryption_level == FD_TLS_LEVEL_APPLICATION));
  sassert(flush == 0 || flush == 1);

  int retval; /* unconstrained */
  return retval;
}

ulong __attribute__((warn_unused_result))
tls_quic_tp_self(void *handshake, uchar *quic_tp, ulong quic_tp_bufsz) {
  sassert(sea_is_dereferenceable(handshake, sizeof(fd_tls_estate_base_t)));
  sassert(sea_is_dereferenceable(quic_tp, quic_tp_bufsz));
  ulong sz;
  assume(sz <= quic_tp_bufsz);
  memhavoc(quic_tp, sz);
  return sz;
}

void tls_quic_tp_peer(void *handshake, uchar const *quic_tp, ulong quic_tp_sz) {
  sassert(sea_is_dereferenceable(handshake, sizeof(fd_tls_estate_base_t)));
  sassert(sea_is_dereferenceable(quic_tp, quic_tp_sz));
}

/* stub fd_tls_sign_fn */
void fd_tls_sign_fn(void *ctx, uchar *sig, uchar const *payload) {
  sassert( ctx == fd_tls_sign_ctx );
  sassert(sea_is_dereferenceable( payload, 130UL ));
  memhavoc( sig, 64UL );
}
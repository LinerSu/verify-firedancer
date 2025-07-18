#ifndef FD_TLS_HELPER_H
#define FD_TLS_HELPER_H

#include "waltz/tls/fd_tls.h"
#include "util/rng/fd_rng.h"

void *tls_rand(void *ctx, void *buf, ulong bufsz);
void tls_secrets(void const *handshake, void const *recv_secret, void const *send_secret, uint encryption_level);
int tls_sendmsg(void const *handshake, void const *record, ulong record_sz, uint encryption_level, int flush);
ulong tls_quic_tp_self(void *handshake, uchar *quic_tp, ulong quic_tp_bufsz) __attribute__((warn_unused_result));
void tls_quic_tp_peer(void *handshake, uchar const *quic_tp, ulong quic_tp_sz);
void fd_tls_sign_fn(void *ctx, uchar *sig, uchar const *payload);

#endif /* FD_TLS_HELPER_H */
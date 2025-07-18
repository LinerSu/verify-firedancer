/* waltz/tls/fd_tls_proto.c stubs fd_tls parsers */
#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "waltz/tls/fd_tls_proto.h"

static long generic_encode(void const *in, ulong in_sz, void *wire,
                           ulong wire_sz) {
  sassert( sea_is_dereferenceable(in, in_sz));
  sassert( sea_is_dereferenceable(wire, wire_sz));

  memhavoc(wire, wire_sz);

  uchar ok = nd_uchar();
  assume(ok <= 1);
  long res = nd_long();
  assume(res > INT_MIN);
  if (ok) {
    assume(wire_sz < LONG_MAX);
    assume(res >= 0L && res <= (long)wire_sz);
  } else {
    assume(res < 0L);
  }

  return res;
}

long fd_tls_encode_client_hello(fd_tls_client_hello_t const *in, uchar *wire,
                                ulong wire_sz) {
  if (in->alpn.bufsz)
    sassert( sea_is_dereferenceable(in->alpn.buf, in->alpn.bufsz));
  if (in->quic_tp.bufsz)
    sassert( sea_is_dereferenceable(in->quic_tp.buf, in->quic_tp.bufsz));
  return generic_encode(in, sizeof(fd_tls_client_hello_t), wire, wire_sz);
}

long fd_tls_decode_client_hello(fd_tls_client_hello_t *out, uchar const *wire,
                                ulong wire_sz) {
  memhavoc(out, sizeof(fd_tls_client_hello_t));

  ulong actual_sz = nd_ulong();
  assume(actual_sz != 0UL);
  if (actual_sz > wire_sz)
    return -1L;
  sassert( sea_is_dereferenceable(wire, actual_sz));
  assume(out->server_name.host_name_len <= 253UL);
  assume(
      out->server_name.host_name_len == 0UL ||
      out->server_name.host_name[out->server_name.host_name_len - 1UL] == '\0');

  ulong quic_tp_off = nd_ulong();
  assume(quic_tp_off < actual_sz);
  ulong quic_tp_sz = nd_ulong();
  assume(quic_tp_sz < actual_sz - quic_tp_off);
  out->quic_tp.buf = wire + quic_tp_off;
  out->quic_tp.bufsz = quic_tp_sz;

  ulong alpn_off = nd_ulong();
  assume(alpn_off < actual_sz);
  ulong alpn_sz = nd_ulong();
  assume(alpn_sz < actual_sz - alpn_off);
  out->alpn.buf = wire + alpn_off;
  out->alpn.bufsz = alpn_sz;

  return actual_sz;
}

long fd_tls_encode_server_hello(fd_tls_server_hello_t const *in, uchar *wire,
                                ulong wire_sz) {
  return generic_encode(in, sizeof(fd_tls_server_hello_t), wire, wire_sz);
}

long fd_tls_decode_server_hello(fd_tls_server_hello_t *out, uchar const *wire,
                                ulong wire_sz) {
  memhavoc(out, sizeof(fd_tls_server_hello_t));

  ulong actual_sz = nd_ulong();
  assume(actual_sz != 0UL);
  if (actual_sz > wire_sz)
    return -1L;
  sassert( sea_is_dereferenceable(wire, actual_sz));
  return actual_sz;
}

long fd_tls_encode_enc_ext(fd_tls_enc_ext_t const *in, uchar *wire,
                           ulong wire_sz) {
  if (in->alpn.bufsz)
    sassert( sea_is_dereferenceable(in->alpn.buf, in->alpn.bufsz));
  if (in->quic_tp.bufsz)
    sassert( sea_is_dereferenceable(in->quic_tp.buf, in->quic_tp.bufsz));
  return generic_encode(in, sizeof(fd_tls_enc_ext_t), wire, wire_sz);
}

long fd_tls_decode_enc_ext(fd_tls_enc_ext_t *out, uchar const *wire,
                           ulong wire_sz) {
  memhavoc(out, sizeof(fd_tls_enc_ext_t));

  ulong actual_sz = nd_ulong();
  assume(actual_sz != 0UL);
  if (actual_sz > wire_sz)
    return -1L;
  sassert( sea_is_dereferenceable(wire, actual_sz));

  ulong quic_tp_off = nd_ulong();
  assume(quic_tp_off < actual_sz);
  ulong quic_tp_sz = nd_ulong();
  assume(quic_tp_sz < actual_sz - quic_tp_off);
  out->quic_tp.buf = wire + quic_tp_off;
  out->quic_tp.bufsz = quic_tp_sz;

  ulong alpn_off = nd_ulong();
  assume(alpn_off < actual_sz);
  ulong alpn_sz = nd_ulong();
  assume(alpn_sz < actual_sz - alpn_off);
  out->alpn.buf = wire + alpn_off;
  out->alpn.bufsz = alpn_sz;

  return actual_sz;
}

long fd_tls_encode_raw_public_key(uchar const *ed25519_pubkey, uchar *wire,
                                  ulong wire_sz) {
  ulong const encoded_sz = 57UL;

  if (wire_sz < encoded_sz)
    return -1L;
  sassert( sea_is_dereferenceable(ed25519_pubkey, 32UL));
  memhavoc(wire, encoded_sz);
  return 57L;
}

long fd_tls_decode_cert_verify(fd_tls_cert_verify_t *out, uchar const *wire,
                               ulong wire_sz) {
  memhavoc(out, sizeof(fd_tls_cert_verify_t));

  ulong actual_sz = nd_ulong();
  assume(actual_sz != 0UL);
  if (actual_sz > wire_sz)
    return -1L;
  sassert( sea_is_dereferenceable(wire, actual_sz));
  out->sig_alg = FD_TLS_SIGNATURE_ED25519;
  return actual_sz;
}

fd_tls_extract_cert_pubkey_res_t
fd_tls_extract_cert_pubkey(uchar const *cert, ulong cert_sz, uint cert_type) {
  sassert((cert_type == FD_TLS_CERTTYPE_RAW_PUBKEY) |
         (cert_type == FD_TLS_CERTTYPE_X509));

  fd_tls_extract_cert_pubkey_res_t res; /* unconstrained */

  uchar ok = 0;
  assume(ok <= 1);
  ok &= (cert_sz >= 32UL);
  if (ok) {
    assume(res.pubkey >= cert && res.pubkey < cert + cert_sz - 32UL);
    res.alert = 0U;
    res.reason = 0U;
  } else {
    /* res.{alert,reason} arbitrary */
    res.pubkey = NULL;
  }

  return res;
}

long fd_tls_encode_cert_verify(fd_tls_cert_verify_t const *in, uchar *wire,
                               ulong wire_sz) {
  return generic_encode(in, sizeof(fd_tls_cert_verify_t), wire, wire_sz);
}

long fd_tls_encode_cert_x509(uchar const *x509, ulong x509_sz, uchar *wire,
                             ulong wire_sz) {
  return generic_encode(x509, x509_sz, wire, wire_sz);
}
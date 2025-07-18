/* ballet/ed25519/fd_ed25519_user.c stubs Ed25519 logic */
#include <seahorn/seahorn.h>
#include <nondet.h>
#include <sea_allocators.h>
#include <proof_allocators.h>
#include <utils.h>

#include "ballet/ed25519/fd_ed25519.h"
#include "ballet/ed25519/fd_x25519.h"
#include "ballet/sha256/fd_sha256.h"
#include "ballet/sha512/fd_sha512.h"

uchar *fd_ed25519_sign(uchar sig[64], uchar const msg[], /* msg_sz */
                       ulong msg_sz, uchar const public_key[32],
                       uchar const private_key[32], fd_sha512_t *sha) {
  sassert( sea_is_dereferenceable(msg, msg_sz));
  sassert( sea_is_dereferenceable(public_key, 32UL));
  sassert( sea_is_dereferenceable(private_key, 32UL));
  sassert( sea_is_dereferenceable(sha, sizeof(fd_sha512_t)));

  sassert( sea_is_dereferenceable(sig, 64UL));
  memhavoc(sig, 64UL);
  return sig;
}

int fd_ed25519_verify(uchar const msg[], ulong sz, uchar const sig[64],
                      uchar const public_key[32], fd_sha512_t *sha) {

  sassert( sea_is_dereferenceable(msg, sz));
  sassert( sea_is_dereferenceable(sig, 64UL));
  sassert( sea_is_dereferenceable(public_key, 32UL));
  sassert( sea_is_dereferenceable(sha, sizeof(fd_sha512_t)));

  int retval = nd_int();
  assume(
      (retval == FD_ED25519_SUCCESS) | (retval == FD_ED25519_ERR_SIG) |
      (retval == FD_ED25519_ERR_PUBKEY) | (retval == FD_ED25519_ERR_MSG));
  return retval;
}

uchar *fd_x25519_exchange(uchar shared_secret[32],
                          uchar const self_private_key[32],
                          uchar const peer_public_key[32]) {
  sassert( sea_is_dereferenceable(self_private_key, 32UL));
  sassert( sea_is_dereferenceable(peer_public_key, 32UL));
  memhavoc(shared_secret, 32UL);
  return shared_secret;
}

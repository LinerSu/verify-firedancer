/* ballet/aes/fd_aes_base_ref.c stubs cryption logic */
#include <seahorn/seahorn.h>
#include <nondet.h>
#include <sea_allocators.h>
#include <proof_allocators.h>
#include <utils.h>

#include "ballet/aes/fd_aes_base.h"
#include "ballet/aes/fd_aes_gcm.h"

int fd_aes_ref_set_encrypt_key( uchar const *  user_key,
                            ulong          bits,
                            fd_aes_key_t * key ) {
  sassert( bits==128UL );
  sassert( sea_is_dereferenceable(user_key, 16) );
  sassert( sea_is_dereferenceable(key, sizeof(fd_aes_key_t)) );
  memhavoc( key, sizeof(fd_aes_key_t) );
  return nd_int();
}

int fd_aes_gcm_decrypt_ref( fd_aes_gcm_ref_t * aes_gcm,
                        uchar const *      c,
                        uchar *            p,
                        ulong              sz,
                        uchar const *      aad,
                        ulong              aad_sz,
                        uchar const        tag[ 16 ] ) {
  sassert( sea_is_dereferenceable(aes_gcm, sizeof(fd_aes_gcm_ref_t)) );
  sassert( sea_is_dereferenceable(c, sz) );
  sassert( sea_is_dereferenceable(p, sz) );
  sassert( sea_is_dereferenceable(aad, aad_sz) );
  return nd_int();
}

/* ballet/sha256/fd_sha256.c stubs SHA256 logic */
#include <seahorn/seahorn.h>
#include <nondet.h>
#include <sea_allocators.h>
#include <proof_allocators.h>
#include <utils.h>

#include "ballet/sha256/fd_sha256.h"

fd_sha256_t *
fd_sha256_init( fd_sha256_t * sha ) {
  sassert( sea_is_dereferenceable( sha, sizeof(fd_sha256_t) ));
  memhavoc( sha, sizeof(fd_sha256_t) );
  return sha;
}

fd_sha256_t *
fd_sha256_append( fd_sha256_t * sha,
                  void const *  data,
                  ulong         sz ) {
  sassert( sea_is_dereferenceable( sha, sizeof(fd_sha256_t) ));
  sassert( sea_is_dereferenceable( data, sz ));
  memhavoc( sha, sizeof(fd_sha256_t) );
  return sha;
}

void *
fd_sha256_fini( fd_sha256_t * sha,
                void *        hash ) {
  sassert( sea_is_dereferenceable( sha, sizeof(fd_sha256_t) ));
  sassert( sea_is_dereferenceable( hash, 32UL ));
  memhavoc( sha, sizeof(fd_sha256_t) );
  memhavoc( hash, 32UL );
  return sha;
}

void *
fd_hmac_sha256( void const * data,
                ulong        data_sz,
                void const * key,
                ulong        key_sz,
                void *       hash ) {
  sassert( sea_is_dereferenceable( data, data_sz ));
  sassert( sea_is_dereferenceable( key,  key_sz  ));
  memhavoc( hash, 32UL );
  return hash;
}

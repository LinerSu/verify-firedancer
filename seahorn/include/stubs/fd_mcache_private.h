#ifndef HEADER_fd_src_tango_fd_mcache_private_h
#define HEADER_fd_src_tango_fd_mcache_private_h

#include "fd_mcache.h"
#ifdef __SEAHORN__
#include <seahorn/seahorn.h>
#include <nondet.h>
#include <stdint.h>

#define MAX_DEPTH 10UL
#define MAX_APP_SZ 10UL
#endif

/* FD_MCACHE_MAGIC is used to signal the layout of shared memory region
   of a mcache. */

#define FD_MCACHE_MAGIC (0xF17EDA2C373CA540UL) /* F17E=FIRE,DA2C/37=DANCER,3C/A54=MCASH,0=V0 / FIRE DANCER MCASH V0 */

/* fd_mcache_private_hdr specifies the detailed layout of the shared
   memory region.  */

struct __attribute__((aligned(FD_MCACHE_ALIGN))) fd_mcache_private_hdr {

  /* This point is FD_MCACHE_ALIGN aligned  */

  ulong magic;   /* == FD_MCACHE_MAGIC */
  ulong depth;   /* == 2^lg_depth >= FD_MCACHE_LG_BLOCK */
  ulong app_sz;  /* Size of the application region in bytes */
  ulong seq0;    /* Initial sequence number passed on creation */
  ulong app_off; /* Location of the application region relative to the first byte of the header */

  /* Padding to FD_MCACHE_ALIGN here (lots of room for additional static data here) */
#ifdef __SEAHORN__
  ulong *seq; /* Explicit storage for seq[] */
#else
  ulong __attribute__((aligned(FD_MCACHE_ALIGN))) seq[ FD_MCACHE_SEQ_CNT ];
#endif

  /* Padding to FD_MCACHE_ALIGN here (probably zero), this is implicitly FD_FRAG_META_ALIGNED */

  /* depth fd_frag_meta_t here */
#ifdef __SEAHORN__
   /* Explicit storage for meta[]: */
   fd_frag_meta_t *meta;
#endif

  /* Padding to FD_MCACHE_ALIGN (probably zero) */

  /* app_off points here */
  /* app_sz byte reserved here */
#ifdef __SEAHORN__
   /* Explicit storage for data[]: */
   uchar *data;
#endif

  /* Padding to FD_MCACHE_ALIGN here */
};

typedef struct fd_mcache_private_hdr fd_mcache_private_hdr_t;

FD_PROTOTYPES_BEGIN

#ifdef __SEAHORN__
extern fd_mcache_private_hdr_t const *glb_mcahe_const_ptr;
extern fd_mcache_private_hdr_t *glb_mcahe_ptr;
FD_FN_CONST static inline fd_frag_meta_t const *
fd_mcache_private_cache_const( fd_mcache_private_hdr_t const * mcache ) {
  glb_mcahe_const_ptr = mcache;
  return mcache->meta;
}

FD_FN_CONST static inline fd_frag_meta_t *
fd_mcache_private_mcache( fd_mcache_private_hdr_t * mcache ) {
  glb_mcahe_ptr = mcache;
  return mcache->meta;
}

FD_FN_CONST static inline fd_mcache_private_hdr_t const *
fd_mcache_private_hdr_const( fd_frag_meta_t const * mcache ) {
  return glb_mcahe_ptr;
}

FD_FN_CONST static inline fd_mcache_private_hdr_t *
fd_mcache_private_hdr( fd_frag_meta_t * mcache ) {
  return glb_mcahe_ptr;
}
#else
FD_FN_CONST static inline fd_frag_meta_t const *
fd_mcache_private_cache_const( fd_mcache_private_hdr_t const * mcache ) {
  return (fd_frag_meta_t const *)(mcache+1UL);
}

FD_FN_CONST static inline fd_frag_meta_t *
fd_mcache_private_mcache( fd_mcache_private_hdr_t * mcache ) {
  return (fd_frag_meta_t *)(mcache+1UL);
}

FD_FN_CONST static inline fd_mcache_private_hdr_t const *
fd_mcache_private_hdr_const( fd_frag_meta_t const * mcache ) {
  return (fd_mcache_private_hdr_t const *)(((ulong)mcache) - sizeof(fd_mcache_private_hdr_t));
}

FD_FN_CONST static inline fd_mcache_private_hdr_t *
fd_mcache_private_hdr( fd_frag_meta_t * mcache ) {
  return (fd_mcache_private_hdr_t *)(((ulong)mcache) - sizeof(fd_mcache_private_hdr_t));
}
#endif

FD_PROTOTYPES_END

#endif /* HEADER_fd_src_tango_fd_mcache_private_h */

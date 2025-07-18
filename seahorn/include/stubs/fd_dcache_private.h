#ifndef HEADER_fd_src_tango_fd_dcache_private_h
#define HEADER_fd_src_tango_fd_dcache_private_h

#include "tango/dcache/fd_dcache.h"
#ifdef __SEAHORN__
#include <seahorn/seahorn.h>
#include <nondet.h>
#include <stdint.h>
#endif

/* FD_DCACHE_MAGIC is used to signal the layout of shared memory region
   of a dcache. */

#define FD_DCACHE_MAGIC (0xF17EDA2C37DCA540UL) /* F17E=FIRE,DA2C/37=DANCER,DC/A54=DCASH,0=V0 / FIRE DANCER MCASH V0 */

/* fd_dcache_private_hdr specifies the detailed layout of the shared
   memory region. */

struct __attribute__((aligned(FD_DCACHE_ALIGN))) fd_dcache_private_hdr {

  /* This point is FD_DCACHE_ALIGN aligned */

  ulong magic;   /* == FD_DCACHE_MAGIC */
  ulong data_sz; /* Size of the data region in bytes */
  ulong app_sz;  /* Size of the application region in bytes */
  ulong app_off; /* Location of the application region relative to first byte of the header */

  /* Padding to FD_DCACHE_ALIGN here */
#ifdef __SEAHORN__
  /* match size of 128 bytes array */
  ulong extra[12UL];
#endif
  uchar __attribute__((aligned(FD_DCACHE_ALIGN))) guard[FD_DCACHE_GUARD_FOOTPRINT];
  /* Padding to FD_DCACHE_ALIGN here (probably zero) */

  /* data_sz bytes here */
#ifdef __SEAHORN__
   /* Explicit storage for data[]: */
   uchar *data;
#endif

  /* Padding to FD_DCACHE_ALIGN here */

  /* app_off points here */
  /* app_sz byte reserved here */
#ifdef __SEAHORN__
   /* Explicit storage for app[]: */
   uchar *app;
#endif
  /* Padding to FD_DCACHE_ALIGN here */
};

typedef struct fd_dcache_private_hdr fd_dcache_private_hdr_t;

FD_PROTOTYPES_BEGIN

#ifdef __SEAHORN__
extern fd_dcache_private_hdr_t const *glb_dcahe_const_ptr;
extern fd_dcache_private_hdr_t *glb_dcahe_ptr;
FD_FN_CONST static inline uchar const *
fd_dcache_private_cache_const(fd_dcache_private_hdr_t const *dcache) {
  glb_dcahe_const_ptr = dcache;
  return glb_dcahe_const_ptr->data;
}

FD_FN_CONST static inline uchar *
fd_dcache_private_dcache(fd_dcache_private_hdr_t *dcache) {
  glb_dcahe_ptr = dcache;
  return glb_dcahe_ptr->data;
}

FD_FN_CONST static inline fd_dcache_private_hdr_t const *
fd_dcache_private_hdr_const(uchar const *dcache) {
  return glb_dcahe_ptr;
}

FD_FN_CONST static inline fd_dcache_private_hdr_t *
fd_dcache_private_hdr(uchar *dcache) {
  return glb_dcahe_ptr;
}
#else
FD_FN_CONST static inline uchar const *
fd_dcache_private_cache_const( fd_dcache_private_hdr_t const * dcache ) {
  return (uchar const *)(dcache+1UL);
}

FD_FN_CONST static inline uchar *
fd_dcache_private_dcache( fd_dcache_private_hdr_t * dcache ) {
  return (uchar *)(dcache+1UL);
}

FD_FN_CONST static inline fd_dcache_private_hdr_t const *
fd_dcache_private_hdr_const( uchar const * dcache ) {
  return (fd_dcache_private_hdr_t const *)(((ulong)dcache) - sizeof(fd_dcache_private_hdr_t));
}

FD_FN_CONST static inline fd_dcache_private_hdr_t *
fd_dcache_private_hdr( uchar * dcache ) {
  return (fd_dcache_private_hdr_t *)(((ulong)dcache) - sizeof(fd_dcache_private_hdr_t));
}
#endif

FD_PROTOTYPES_END

#endif /* HEADER_fd_src_tango_fd_dcache_private_h */

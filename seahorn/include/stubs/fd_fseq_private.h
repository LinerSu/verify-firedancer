#include "fd_fseq.h"

/*
   NOTE: the following is defined in the fd_fseq.c file to hidden internal structure.
   We external here just for verification.
*/
#ifdef __SEAHORN__
#define FD_FSEQ_MAGIC (0xf17eda2c37f5ec00UL)
struct __attribute__((aligned(FD_FSEQ_ALIGN))) fd_fseq_shmem {
  ulong magic; /* == FD_FSEQ_MAGIC */
  ulong seq0;  /* Initial sequence number */
  ulong seq;   /* Current sequence number */
  /* Padding to FD_FSEQ_APP_ALIGN here */
  /* FD_FSEQ_APP_FOOTPRINT for app region here */
  /* Padding to FD_FSEQ_ALIGN here */
  // WARN: seadsa infers this as a sequence of ulong.
  //       We create a stub to avoid this.
};
typedef struct fd_fseq_shmem fd_fseq_shmem_t;
ulong *initialize_bounded_seq(ulong size, ulong seq0);
void allocate_fseq(void);

extern ulong *g_ul_ptr;
extern fd_fseq_shmem_t *hidden;
#endif
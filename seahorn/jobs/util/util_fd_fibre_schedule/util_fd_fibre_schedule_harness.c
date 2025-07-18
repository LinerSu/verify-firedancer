#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>
#include <bounds.h>

#include "util/fibre/fd_fibre.h"

typedef struct {
    int        fd;          // e.g. socket or file descriptor
    void      *buf;         // pointer to data buffer
    size_t     len;         // length of buffer
    void      *user_tag;    // opaque user data
} fd_arg_t;

void init_arg(fd_arg_t *a) {
  a = bounded_malloc_havoc(sizeof(fd_arg_t));
  a->fd       = nd_int();
  a->len      = nd_size_t();
  assume(a->len <= sea_max_buffer_size());
  CRAB_ASSUME(a->len > 0); // Ensure length is at least 1 byte
  a->buf      = bounded_malloc_havoc(a->len);
  a->user_tag = nd_voidp();
}

void fn2(void *vp) { 
  sassert(vp == NULL);
  sassert(sea_is_dereferenceable(vp, sizeof(fd_arg_t)));
}

int main(void) {
  // initialize fibres
  void *main_fibre_mem = bounded_malloc_havoc(fd_fibre_init_footprint()); // 1152
  fd_fibre_t *main_fibre = fd_fibre_init(main_fibre_mem);
  fd_arg_t arg;
  init_arg(&arg);
  sassert(main_fibre);

  ulong stack_sz = nd_ulong();
  CRAB_ASSUME(stack_sz > 0); // Ensure stack size is at least 1 byte
  assume(stack_sz <= 1024);
  void *fibre_1_mem = bounded_malloc_havoc(
      fd_fibre_start_footprint(stack_sz)); // 1152 + stack_sz
  fd_fibre_t *t0 = fd_fibre_start(fibre_1_mem, stack_sz, fn2, &arg);

  fd_fibre_schedule( t0 );

  // run schedule until done
  long now = nd_long(); long old_now = now;
  long old_timeout = 0;
  while( 1 ) {
    long timeout = fd_fibre_schedule_run();
    if( timeout == -1 ) {
      /* -1 indicates no fibres scheduled */
      break;
    }
    old_timeout = timeout;

    now = timeout;
  }

  sassert(fd_fibre_current == main_fibre);
  sassert(now == old_now + old_timeout);
  fd_fibre_free(t0);
  return 0;
}

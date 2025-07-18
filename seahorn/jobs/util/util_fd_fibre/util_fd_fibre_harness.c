#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>

#include "util/fibre/fd_fibre.h"
void fn1(void *vp) { sassert(vp == NULL); }

int main(void) {
  // initialize fibres
  void *main_fibre_mem = bounded_malloc_havoc(fd_fibre_init_footprint()); // 1152
  fd_fibre_t *main_fibre = fd_fibre_init(main_fibre_mem);

  sassert(fd_fibre_current == main_fibre);

  ulong stack_sz = nd_ulong();
  CRAB_ASSUME(stack_sz > 0); // Ensure stack size is at least 1 byte
  assume(stack_sz <= 1024);
  void *fibre_1_mem = bounded_malloc_havoc(
      fd_fibre_start_footprint(stack_sz)); // 1152 + stack_sz
  fd_fibre_t *fibre_1 = fd_fibre_start(fibre_1_mem, stack_sz, fn1, NULL);
  sea_tracking_on();

  fd_fibre_swap( fibre_1 );
  sassert(sea_is_modified((char *)&fibre_1->ctx));
  sassert(fd_fibre_current == main_fibre);

  sea_tracking_off();
  fd_fibre_free(fibre_1);
  return 0;
}

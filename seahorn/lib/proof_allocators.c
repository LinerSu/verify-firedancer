/*
 *
 */

#include <proof_allocators.h>

#include <stdarg.h>
#include <stdlib.h>

#include <nondet.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>

void *realloc(void *ptr, size_t new_size) { return sea_realloc(ptr, new_size); }

void *bounded_malloc_havoc(size_t size) {
  #ifdef __CRAB__
  assume(size > 0);
  #endif
  return size == 0 ? NULL : sea_malloc_havoc_safe(size);
}

void *can_fail_malloc_havoc(size_t size) {
  #ifdef __CRAB__
  assume(size > 0);
  #endif
  return size == 0 ? NULL : sea_malloc_havoc(size);
}

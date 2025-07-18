/* POSIX C library context control stubs */
#include <seahorn/seahorn.h>
#include <nondet.h>
#include <sea_allocators.h>
#include <proof_allocators.h>
#include <utils.h>

#include <errno.h>

int *__errno_location (void) {
  return NULL;
}
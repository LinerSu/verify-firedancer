/* POSIX C library context control stubs */
#include <seahorn/seahorn.h>
#include <nondet.h>
#include <sea_allocators.h>
#include <proof_allocators.h>
#include <utils.h>

#include <ucontext.h>

static ucontext_t ctx_fibre1, ctx_fibre2;

int getcontext(ucontext_t *ucp) {
  if (!ucp) {
    ucp = nd_bool() ? &ctx_fibre1 : &ctx_fibre2; // Use a static context if none provided
  }
  sassert(ucp);
  sassert(sea_is_dereferenceable(ucp, sizeof(ucontext_t)));
  memhavoc(ucp, sizeof(ucontext_t));
  int ret = nd_int();
  assume(ret != -1); // Assume getcontext does not fail
  return ret; // Simulate a successful or failed getcontext call
}

int swapcontext(ucontext_t *oucp, const ucontext_t *ucp) {
  sassert(sea_is_dereferenceable(oucp, sizeof(ucontext_t)));
  sassert(sea_is_dereferenceable(ucp, sizeof(ucontext_t)));
  int ret = nd_int();
  assume(ret != -1); // Assume getcontext does not fail
  return ret;        // Simulate a successful or failed getcontext call
}
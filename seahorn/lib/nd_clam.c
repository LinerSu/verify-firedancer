#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <seahorn/seahorn.h>
#include "nondet.h"

extern NONDET_FN_ATTR int64_t nd_int64_t(void);
extern NONDET_FN_ATTR int8_t  nd_int8_t(void);
extern NONDET_FN_ATTR int16_t nd_int16_t(void);
extern NONDET_FN_ATTR int32_t nd_int32_t(void);

bool nd_malloc_is_fail(void) {
  // make assumption for crab
  // malloc always safe
  return false;
}

size_t nd_size_t(void) {
  int64_t res = nd_int64_t();
  __VERIFIER_assume(res >= 0);
  return (size_t)res;
}  

uint8_t nd_uint8_t(void) {
  int8_t res = nd_int8_t();
  __VERIFIER_assume(res >= 0);
  return (uint8_t)res;  
}

uint16_t nd_uint16_t(void) {
  int16_t res = nd_int16_t();
  __VERIFIER_assume(res >= 0);
  return (uint16_t)res;  
}

uint32_t nd_uint32_t(void) {
  int32_t res = nd_int32_t();
  __VERIFIER_assume(res >= 0);
  return (uint32_t)res;  
}

uint64_t nd_uint64_t(void) {
  int64_t res = nd_int64_t();
  __VERIFIER_assume(res >= 0);
  return (uint64_t)res;    
}

unsigned long nd_ulong(void) {
#if ULONG_MAX == 0xFFFFFFFFUL
  return (unsigned long) nd_uint32_t();
#elif ULONG_MAX == 0xFFFFFFFFFFFFFFFFUL
  return (unsigned long) nd_uint64_t();
#else
  #error "Unsupported unsigned long size"
#endif
}

unsigned char nd_uchar(void) {
  return (unsigned char) nd_uint8_t();
}

long nd_long(void) {
#if LONG_MAX == 0x7FFFFFFFL
  return (long) nd_int32_t();
#elif LONG_MAX == 0x7FFFFFFFFFFFFFFFL
  return (long) nd_int64_t();
#else
  #error "Unsupported long size"
#endif
}

unsigned int nd_uint(void) {
#if UINT_MAX == 0xFFFFFFFFUL
  return (unsigned int) nd_uint32_t();
#elif UINT_MAX == 0xFFFFFFFFFFFFFFFFUL
  return (unsigned int) nd_uint64_t();
#else
  #error "Unsupported unsigned int size"
#endif
}
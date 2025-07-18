/*
 *
 */

#pragma once

#include <seahorn/seahorn.h>

#include <nondet.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __CRAB__
#define CRAB_ASSUME(X) assume(X)
#else
#define CRAB_ASSUME(X)
#endif

extern __declspec(noalias) void sea_printf(const char *format, ...);

#define IMPLIES(a, b) (!(a) || (b))
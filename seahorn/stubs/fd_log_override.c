#include <seahorn/seahorn.h>
#include <nondet.h>
#include <stdlib.h>

char const *fd_log_private_0(char const *fmt, ...) {
  (void)fmt;
  return "";
}

void fd_log_private_1(int level, long now, char const *file, int line,
                      char const *func, char const *msg) {
  (void)now;
  (void)func;
  exit(0);
}

void fd_log_private_2(int level, long now, char const *file, int line,
                      char const *func, char const *msg) {
  (void)now;
  (void)func;
  exit(0);
}

long current_wallclock = 0;

long fd_log_wallclock(void) {
  long t = nd_long();
  assume(t >= current_wallclock);
  current_wallclock = t;
  return current_wallclock;
}
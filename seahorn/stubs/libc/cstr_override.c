/* C library string function stubs */
#include <nondet.h>
#include <proof_allocators.h>
#include <sea_allocators.h>
#include <seahorn/seahorn.h>
#include <utils.h>
#include <bounds.h>

#include <stdio.h>
#include <ctype.h>

const unsigned short int dummy_ctype_table[256];

const unsigned short int **__ctype_b_loc(void) {
    // Return the address of a pointer to our dummy table
    memhavoc((void *)dummy_ctype_table, sizeof(unsigned short int) * 256);
    static const unsigned short int *table_ptr = dummy_ctype_table;
    return &table_ptr;
}

unsigned long strtoul(const char *str, char **endptr, int base) {
  sassert(base == 0 || base == 10); // Simplify base handling

  unsigned long result = nd_ulong();

  char *str1 = (char *)str;

  // unroll first
  for (int i = 0; i < sea_max_buffer_size(); i++) {
    if (! (isspace((unsigned char)*str1) || isdigit((unsigned char)*str1)) ) {
        break;
    }
    str1++;
  }

  // Set endptr if not NULL
  if (endptr) {
      *endptr = (char *)str1;
  }

  return result;
}
#ifndef _PAGE_GEN_INCLUDE_
#define _PAGE_GEN_INCLUDE_

#include <stdint.h>

typedef struct {
  const char *titl;
  const char *name;
  const char *syml;
  const char *desc;
} pf;

typedef int(*wr_s )(const char*, ...);
typedef int(*wr_fd)(int, size_t);

#ifndef _PAGE_GEN_IMPLEMENTATION_
extern int generate_page(wr_s, wr_fd, size_t);
#endif // _PAGE_GEN_IMPLEMENTATION_

#endif // _PAGE_GEN_INCLUDE_
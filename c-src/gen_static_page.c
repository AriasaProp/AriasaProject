#ifndef PROJECT_NAME
  #error "please define PROJECT_NAME"
#endif
// #define SHRINK_PAGE
#include <stddef.h>
#include "log.h"
#include "page_desc.h"
#include "page_gen.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

int out;
static int write_string(const char *s, ...) {
  va_list args;
  va_start(args, s);
  int result = vdprintf(out, s, args);
  va_end(args);
  return result <= 0;
}
static int write_descriptor(int fd, size_t l) {
  return sendfile(out, fd, NULL, l) != l;
}

#define TEMPZ 256
static char temps[TEMPZ];
int main (int argc, char **argv) {
  const char *dirname = (argc < 2) ? "" : argv[1];
  for (size_t page_i = 0; page_i < sizeof(page_list)/sizeof(pf); ++page_i) {
    pf current_page = page_list[page_i];
    sprintf(temps, "%s/%s.html", dirname, current_page.name);
    LOGI("Generate %s: \n", temps);
    out = creat(temps, S_IRUSR | S_IWUSR);
    if (out < 0) {
      LOGE("Fail to create %s.\n", temps);
      return -1;
    }
    generate_page(write_string, write_descriptor, page_i);
    close(out);
  }
  return 0;
}
#undef TEMPZ

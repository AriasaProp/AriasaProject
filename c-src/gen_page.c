#ifndef PROJECT_NAME
  #error "please define PROJECT_NAME"
#endif
// #define SHRINK_PAGE
#include <stddef.h>
#include "page_template.h"
#include "page_desc.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main (int, char **) {
#define TEMPZ 2048
  char *temps = (char*)malloc(TEMPZ);
  int out, in;
  struct stat st;
  for (size_t page_i = 0; page_i < sizeof(page_list)/sizeof(pf); ++page_i) {
    pf current_page = page_list[page_i];
    sprintf(temps, "%s.html", current_page.name);
    printf("Generate %s: ", temps);
    out = creat(temps, S_IRUSR | S_IWUSR);
    if (out < 0) {
      printf("Fail to open %s.\n", temps);
      continue;
    }
    dprintf(out, base_page_template[0], current_page.titl, current_page.desc, current_page.name, page_i ? "" : " href=\'/\'");
    // input shortcuts 
    for (size_t page_j = 1; page_j < sizeof(page_list)/sizeof(pf); ++page_j) {
      if (page_i == page_j) continue;
      pf page_link = page_list[page_j];
      sprintf(temps, "%s.html", page_link.name);
      dprintf(out, base_page_template[1], temps, page_link.syml);
    }
    dprintf(out, base_page_template[2]);
    // docs chtml input
    sprintf(temps, "page-src/%s.chtml", current_page.name);
    in = open(temps, O_RDONLY);
    if (in > 0) {
      if (fstat(in, &st))
        printf("Fail to get fstat on file");
      else if (sendfile(out, in, NULL, st.st_size) < 0)
        printf("Fail to sendfile %s", strerror(errno));
      // todo: need to handle when sendfile not fully copy
      close(in);
      dprintf(out, base_page_template[3]);
      printf("Success.\n");
    } else printf("Fail to open %s", temps);
    close(out);
  }
  free(temps);
  return 0;
#undef TEMPZ
}

#ifndef PROJECT_NAME
  #error "please define PROJECT_NAME"
#endif
#define _PAGE_GEN_IMPLEMENTATION_

#include <stddef.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "log.h"
#include "page_gen.h"
#include "page_template.h"
#include "page_desc.h"

#define TEMPZ 2048
static char temps[TEMPZ];

int generate_page(wr_s wrs, wr_fd wrfd, size_t page_i) {
  int in, ret = -1;
  struct stat st;
  pf current_page = page_list[page_i];
  if (wrs(base_page_template[0], current_page.titl, current_page.desc, current_page.name, page_i ? " href=\'/\'" : "")) {
    LOGE("Fail to write wrs");
    goto gp_end;
  }
  // input shortcuts 
  for (size_t page_j = 1; page_j < sizeof(page_list)/sizeof(pf); ++page_j) {
    if (page_i == page_j) continue;
    pf page_link = page_list[page_j];
    sprintf(temps, "%s.html", page_link.name);
    if (wrs(base_page_template[1], temps, page_link.syml)) {
      LOGE("Fail to write wrs");
      goto gp_end;
    }
  }
  if (wrs("%s", base_page_template[2])) {
    LOGE("Fail to write wrs");
    goto gp_end;
  }
  // docs chtml input
  sprintf(temps, "page-src/%s.chtml", current_page.name);
  in = open(temps, O_RDONLY);
  if (in < 0) {
    LOGE("Fail to open %s", temps);
    goto gp_cls;
  }
  if (fstat(in, &st)) {
    LOGE("Fail to get fstat on file");
    goto gp_cls;
  }
  if (wrfd(in, st.st_size) < 0) {
    TODO("need to handle when sendfile not fully copy");
    LOGE("Fail to sendfile %s", strerror(errno));
    goto gp_cls;
  }
  wrs("%s", base_page_template[3]);
  LOGI("Success");
  ret = 0;
gp_cls:
  close(in);
gp_end:
  return ret;
}
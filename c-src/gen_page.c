#ifndef PROJECT_NAME
  #error "please define PROJECT_NAME"
#endif

#define NEWLINE "\n"
#define TB      "  "
#define TB2X    "    "
#define TB3XXX  "      "
#define TB4XXXX "        "
#include "page_template.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define CW(A,B,C,D) (A | (B << 8) | (C << 16) | (D << 24))
typedef enum : int {
  CNT_TITL = CW('t','i','t','l'),
  CNT_DESC = CW('d','e','s','c'),
  CNT_SHRT = CW('s','h','r','t'),
} cntId;
#undef CW
typedef struct {
  cntId id;
  char *str;
} Content;
typedef struct {
  Content *dat;
  size_t len, cap;
} Contents;
static char *contents_take(Contents *c, cntId id) {
  size_t i;
  for (i = 0; i < c->len; ++i) if (c->dat[i].id == id) {
    char *rc = c->dat[i].str;
    --(c->len);
    if (c->len != i)
      memcpy(c->dat + i, c->dat + i + 1, sizeof(Content) * ((c->len) - i));
    return rc;
  }
  return NULL;
}

int main (int argc, char **argv) {
  if (argc < 2) {
    printf("At least give me a name.\n");
    return 1;
  }
#define TEMPZ 2048
  char *temps = (char*)malloc(TEMPZ);
  int ret = 1;
  int st = 0;
  Contents cnts = {0};
  size_t i;
  { // load content file 
    sprintf(temps, "page-src/%s.desc", argv[1]);
    FILE *f = fopen(temps, "r");
    if (!f) {
      printf("Fail to load %s.\n", temps);
      goto end;
    }
    while(!feof(f) && (fread(&st, 1, 4, f) == 4)) {
      switch (st) {
        case CNT_TITL:
        case CNT_DESC:
          for (i = 0; i < cnts.len; ++i) if (cnts.dat[i].id == st) {
            printf("Failure content duplicated.\n");
            goto end_cnt;
          }
          if (fscanf(f, ": %[^\n]\n", temps) <= 0) {
            printf("Failure content format.\n");
            goto end_cnt;
          }
          break;
        case CNT_SHRT:
          if (fscanf(f, ": %[^\n]\n", temps) <= 0) {
            printf("Failure content format.\n");
            goto end_cnt;
          }
          break;
      }
      // Contents reserve
      if (cnts.cap <= cnts.len) {
        if (cnts.cap) do { cnts.cap <<= 1; } while (cnts.cap <= cnts.len);
        else cnts.cap = 4;
        cnts.dat = (Content*)realloc(cnts.dat, cnts.cap * sizeof(Content));
      }
      // Contents append
      cnts.dat[cnts.len].id = st;
      cnts.dat[cnts.len].str = strdup(temps);
      ++cnts.len;
    }
end_cnt:
    fclose(f);
    if (!st) {
      for (i = 0; i < cnts.len; ++i)
        free((void*)cnts.dat[i].str);
      free(cnts.dat);
      memset(&cnts, 0, sizeof(Contents));
      goto end;
    }
  }
  sprintf(temps, "%s.html", argv[1]);
  int out = creat(temps, S_IRUSR | S_IWUSR);
  if (out < 0) {
    printf("Fail to open %s.\n", temps);
    goto end;
  }
  printf("start generate pages.\n");
  {
    char *desc = contents_take(&cnts, CNT_DESC);
    if (!desc) {
      printf("Content doesn\'t have description");
      goto close;
    }
    {
      char *titl = contents_take(&cnts, CNT_TITL);
      if (!titl) {
        temps[0] = 0;
      } else {
        sprintf(temps, " - %s", t1);
        free((void*)titl);
      }
    }
    int isIndex = !memcmp(argv[1],"index", 5);
    dprintf(out, base_page_template[0],temps, desc, argv[1],isIndex ? "" : " href=\'/\'");
    free ((void*)desc);
  }
  // input shortcuts 
  for (char *t1; (t1 = contents_take(&cnts, CNT_SHRT)); ) {
    char *t2 = strchr(t1, ' ');
    if (t2) {
      *(t2++) = 0;
      dprintf(out, base_page_template[1], t1,t2);
    }
    free ((void*)t1);
  }
  dprintf(out, base_page_template[2]);
  { // docs chtml input
    sprintf(temps, "page-src/%s.chtml", argv[1]);
    int in = open(temps, O_RDONLY);
    if (in < 0) {
      printf("Fail to open %s", temps);
      goto close;
    }
    struct stat st;
    if (fstat(in, &st) < 0) {
      printf("Fail to get fstat on content file");
      goto close;
    }
    if (sendfile(out, in, NULL, st.st_size) < 0) {
      printf("Fail to sendfile %s", strerror(errno));
      close(in);
      goto close;
    }
    close(in);
  }
  dprintf(out, base_page_template[3]);
  ret = 0;
  printf("end generate pages.\n");
close:
  close(out);
  for (i = 0; i < cnts.len; ++i)
    free((void*)cnts.dat[i].str);
  free(cnts.dat);
end:
  free(temps);
  return ret;
#undef TEMPZ
}

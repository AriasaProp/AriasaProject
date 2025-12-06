#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef PROJECT_NAME
  #error "please define PROJECT_NAME"
#endif
#define CW(A,B,C,D) (A | (B << 8) | (C << 16) | (D << 24))
typedef enum : int {
  CNT_TITL = CW('t','i','t','l'),
  CNT_DESC = CW('d','e','s','c'),
  CNT_SHRT = CW('s','h','r','t'),
  CNT_DOCS = CW('d','o','c','s'),
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
  int ret = 1, st;
  Contents cnts = {0};
  size_t i;
#define TEMPZ 2048
  char *temps = (char*)malloc(TEMPZ);
  { // load content file 
    sprintf(temps, "page-src/%s.chtml", argv[1]);
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
        case CNT_DOCS:
          (void)fread(temps, 1, 2, f);
          if ((i = fread(temps, 1, TEMPZ, f)) < TEMPZ) {
            temps[i] = 0;
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
  sprintf(temps, "page-src/template.thtml");
  FILE *ftempl = fopen(temps, "r");
  if (!ftempl) {
    printf("Fail to load %s.\n", temps);
    goto end;
  }
  sprintf(temps, "%s.html", argv[1]);
  FILE *out = fopen(temps, "w");
  if (!out) {
    fclose(ftempl);
    printf("Fail to write %s.\n", temps);
    goto end;
  }
#define THIS_INDEX 1
  int type = 
    (!memcmp(argv[1],"index", 5)) * THIS_INDEX;

  st = 0;
  char c;
  char *t;
  printf("start generate pages.\n");
  do {
    if ((c = fgetc(ftempl)) != '%') {
      fputc(c,out);
      continue;
    }
    if ((c = fgetc(ftempl)) != '%') {
      fputc('%',out);
      fputc(c,out);
      continue;
    }
    switch (st++) {                                                                                                                                    
      case 0:
        fputs(PROJECT_NAME, out);
        if ((t = contents_take(&cnts, CNT_TITL))) {
          fprintf(out, " - %s", t);
          free ((void*)t);
        }
        break;
      case 1:
        if ((t = contents_take(&cnts, CNT_DESC))) {
          fprintf(out, " - %s", t);
          free ((void*)t);
        }
        break;
      case 2:
      case 3:
      case 4:
        fputs(argv[1], out);
        break;
      case 5:
        if (!(type & THIS_INDEX)) fputs(" href=\'/\'", out);
        break;
      case 6:
        fputs(PROJECT_NAME, out);
        break;
      case 7: {
        char *t1, *t2, *t3;
        while ((t = contents_take(&cnts, CNT_SHRT))) {
          t1 = t;
          t2 = strchr(t1, ' ');
          if (!t2) { free ((void*)t); continue; }
          *(t2++) = 0;
          t3 = strchr(t2, ' ');
          if (!t3) { free ((void*)t); continue; }
          *(t3++) = 0;
          fprintf(out, "          <li><a href=\'%s\'><i class=\'material-symbols-outlined\'>%s</i><label class=\'hide-on-small\'>%s</label></a></li>", t1,t2,t3);
          free ((void*)t);
        }
      } break;
      case 8:
        if((t = contents_take(&cnts, CNT_DOCS))) {
          fputs(t, out);
          free((void*)t);
        }
        break;
      default: goto close;
    }   
  } while (!feof(ftempl));
  ret = 0;
  printf("end generate pages.\n");
close:
  fclose(ftempl);
  fclose(out);
  for (i = 0; i < cnts.len; ++i)
    free((void*)cnts.dat[i].str);
  free(cnts.dat);
end:
  free(temps);
  return ret;
#undef TEMPZ
}

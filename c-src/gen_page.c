#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef PROJECT_NAME
  #error "please define PROJECT_NAME"
#endif

int main (int argc, char **argv) {
  if (argc < 2) {
    printf("At least give me a name.\n");
    return 1;
  }
  int ret = 1;
  char *temps = (char*)malloc(2048);
  sprintf(temps, "page-src/template.thtml");
  FILE *ftempl = fopen(temps, "r");
  if (!ftempl) {
    printf("Fail to load %s.\n", temps);
    goto end;
  }
  sprintf(temps, "page-src/%s.chtml", argv[1]);
  FILE *fcontent = fopen(temps, "r");
  if (!fcontent) {
    fclose(ftempl);
    printf("Fail to load %s.\n", temps);
    goto end;
  }
  sprintf(temps, "%s.html", argv[1]);
  FILE *out = fopen(temps, "w");
  if (!out) {
    fclose(ftempl);
    fclose(fcontent);
    printf("Fail to write %s.\n", temps);
    goto end;
  }
#define THIS_INDEX 1
  int type = 
    (!memcmp(argv[1],"index", 5)) * THIS_INDEX;

  int state = 0;
  size_t linel;
  char c;
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
    switch (state++) {                                                                                                                                    
      case 0:
        if ((getline(&temps, &linel, fcontent) > 0) && (linel > 0)) {
          temps[linel - 1] = '\0';
          fputs(PROJECT_NAME, out);
          fputs(temps, out);
        }
        break;
      case 1:
      case 4:
        if ((getline(&temps, &linel, fcontent) > 0) && (linel > 0)) {
          temps[linel - 1] = '\0';
          fputs(temps, out);
        }
        break;
      case 2:
      case 3:
        fputs(argv[1], out);
        break;
      case 5:
        if (!(type & THIS_INDEX)) fputs(" href=\'/\'", out);
        break;
      case 6:
        fputs(PROJECT_NAME, out);
        break;
      case 7:
        while ((getline(&temps, &linel, fcontent) > 0) && (linel > 0) && memcmp(temps, "\%END", 4)) {
          temps[linel - 1] = '\0';
          fputs("        ", out);
          fputs(temps, out);
        }
        break;
      case 8:
        while ((getline(&temps, &linel, fcontent) > 0) && (linel > 0)) {
          temps[linel - 1] = '\0';
          fputs("    ", out);
          fputs(temps, out);
        }
        break;
      default: goto close;
    }   
  } while (!feof(ftempl));
  ret = 0;
  printf("end generate pages.\n");
close:
  fclose(ftempl);
  fclose(fcontent);
  fclose(out);
end:
  free(temps);
  return ret;
}

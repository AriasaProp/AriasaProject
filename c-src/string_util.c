#define _STRING_UTIL_IMPLEMENTATION_
#include "string_util.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define STRING_TEMP_LEN 4096

static char string_temp[STRING_TEMP_LEN];

StringView strvdup_temp(const char *s, ...) {
  va_list args;
  va_start(args, s);
  int result = vsnprintf(string_temp, STRING_TEMP_LEN, s, args);
  va_end(args);
  return (StringView){string_temp, strlen(string_temp)};
}
StringView strvdup_buff(const char * const s, const size_t l) {
  return (StringView){s, l};
}
int strv_cmp(StringView a, StringView b) {
  int t = (a.l - b.l);
  if (t) return t;
  return strcmp(a.str, b.str, a.l);
}
#ifndef _STRING_UTIL_INCLUDE_
#define _STRING_UTIL_INCLUDE_

#include <stdint.h>

// only view string from other buffer
typedef struct {
  const char * const str;
  const size_t len;
} StringView;

#ifndef _STRING_UTIL_IMPLEMENTATION_
// store formated string temporary 
extern StringView strvdup_temp(const char *, ...);
extern StringView strvdup_buff(const char * const, const size_t);
extern int        strv_cmp    (StringView, StringView);
#endif // _STRING_UTIL_IMPLEMENTATION_

#endif // _STRING_UTIL_INCLUDE_
#ifndef _COMMON_INCLUDE_
#define _COMMON_INCLUDE_

#define TODO(X)
#define PRIVATE_STRINGIFY(X) #X
#define STRINGIFY(X)  PRIVATE_STRINGIFY(X)

#define UNUSED(x)     ((void)x)

#if defined(_MSC_VER)
  #define UNUSED_ARG(x) __pragma(warning(suppress : 4100 4101)) x
#elif defined(__GNUC__)
  #define UNUSED_ARG(x) __attribute__((unused)) x
#else
  #define UNUSED_ARG(x) 
#endif

#if __GNUC__ >= 3
  #define IS_ERROR(x) if (__builtin_expect(!!(x), 0))
  #define LIKELY(x)   __builtin_expect(!!(x), 1)
  #define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
  #define IS_ERROR(x) if (!!(x))
  #define LIKELY(x)   (x)
  #define UNLIKELY(x) (x)
#endif

#endif // _COMMON_INCLUDE_
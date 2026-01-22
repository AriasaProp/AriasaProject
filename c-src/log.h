#ifndef _LOG_INCLUDED_
#define _LOG_INCLUDED_

#include <stdio.h>
#include <stdarg.h>

#define LOGI(...) printf("\033[34mINFO:\033[0m "), printf(__VA_ARGS__), printf("\n")
#define LOGW(...) printf("\033[33mWARN:\033[0m "), printf(__VA_ARGS__), printf("\n")
#define LOGE(...) printf("\033[31mERRO:\033[0m "), printf(__VA_ARGS__), printf("\n")

#endif  // _LOG_INCLUDED_
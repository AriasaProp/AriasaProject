#ifndef _PAGE_DESC_INCLUDE_
#define _PAGE_DESC_INCLUDE_

#include "page_gen.h"

static const struct {
  const char *path;
  size_t index;
} urlPath_pages[] = {
  {"/"        ,0},
  {"/index"   ,0},
  {"/hompage" ,0},
  {"/pref"    ,1},
  {"/empty"   ,2},
  {"/game"    ,3},
};

static const pf page_list[] = {
  {
    .titl = PROJECT_NAME,
    .name = "index",
    .syml = NULL,
    .desc = "Homepage & Introduction feature."
  },
  {
    .titl = PROJECT_NAME" - Preference",
    .name = "pref",
    .syml = "&#xf859",
    .desc = "Preference Theme."
  },
  {
    .titl = PROJECT_NAME" - Empty",
    .name = "empty",
    .syml = "&#xf859",
    .desc = "Empty page."
  },
  {
    .titl = PROJECT_NAME" - Game",
    .name = "game",
    .syml = "&#xea28",
    .desc = "Game page."
  },
};

#endif // _PAGE_DESC_INCLUDE_
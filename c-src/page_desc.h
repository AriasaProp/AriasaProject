
typedef struct {
  const char *titl;
  const char *name;
  const char *syml;
  const char *desc;
} pf;
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
};
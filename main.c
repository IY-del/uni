#include <stdio.h>

#include "fontlib.h"

int main(void) {
  int (*patterns)[PATTERN_DEFAULT_H][PATTERN_DEFAULT_W] = NULL;
  size_t n = 0;

  int ret = load_txt_default("font_data02.txt", &patterns, &n);
  if (ret != 0) {
    fprintf(stderr, "failed to load font data: %d\n", ret);
    return 1;
  }

  PatternView view = new_pattern_view_doublescale(n, patterns);
  print_as_pattern(&view);

  free_memory(patterns);
  return 0;
}

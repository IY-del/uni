#include <stdio.h>

#include "fontlib.h"

int main(void) {
  int patterns[1][8][8] = {{{0, 0, 1, 1, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0, 0, 0, 0},
                            {1, 1, 1, 1, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0}}};

  /* 1. new_pattern_view / new_pattern_view_doublescale の検証 */
  PatternView view = new_pattern_view(1, patterns);
  printf("normal: h=%zu w=%zu scale_w=%u scale_h=%u rotation=%d\n", view.h,
         view.w, view.scale_w, view.scale_h, view.rotation);

  PatternView view2x = new_pattern_view_doublescale(1, patterns);
  printf("double: h=%zu w=%zu scale_w=%u scale_h=%u rotation=%d\n", view2x.h,
         view2x.w, view2x.scale_w, view2x.scale_h, view2x.rotation);

  /* 2. print_as_pattern の検証 */
  printf("\nnormal:\n");
  print_as_pattern(&view);

  printf("double:\n");
  print_as_pattern(&view2x);

  return 0;
}

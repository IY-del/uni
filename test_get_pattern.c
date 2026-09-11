#include <stdio.h>

#include "fontlib.h"

int main(void) {
  int (*patterns)[8][8];
  size_t n;

  /* load_txt_to_8x8 のテスト */
  if (load_txt_to_8x8("font_data01.txt", &patterns, &n) != 0) {
    fprintf(stderr, "load_txt_to_8x8 failed (font_data01.txt)\n");
    return 1;
  }

  printf("n = %zu\n", n);

  if (n > 0) {
    for (size_t y = 0; y < 8; y++) {
      for (size_t x = 0; x < 8; x++) {
        printf("%d", patterns[0][y][x]);
      }
      putchar('\n');
    }
  }

  /* free_memory のテスト */
  free_memory(patterns);

  return 0;
}

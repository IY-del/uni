#include <stdio.h>
#include <stdlib.h>

#include "fontlib.h"

int main(void) {
  printf("=== 2. パターン読み込み・メモリ管理機能 ===\n");

  /* 1. load_txt のテスト */
  printf("[load_txt]\n");
  int* flat = NULL;
  size_t flat_n = 0;
  int ret_flat = load_txt("font_data01.txt", PATTERN_DEFAULT_H,
                          PATTERN_DEFAULT_W, &flat, &flat_n);
  printf(
      "  load_txt(\"font_data01.txt\", %d, %d): ret=%d, n=%zu (expected: "
      "ret=0, n=26)\n",
      PATTERN_DEFAULT_H, PATTERN_DEFAULT_W, ret_flat, flat_n);
  if (ret_flat == 0) {
    printf("  first letter top row: %d%d%d%d%d%d%d%d (expected: 00011000)\n",
           flat[0], flat[1], flat[2], flat[3], flat[4], flat[5], flat[6],
           flat[7]);
    free_memory(flat);
  }

  /* 異常系・境界値テスト */
  int* err_ptr = NULL;
  size_t err_n = 0;
  int ret_err1 = load_txt("non_existent.txt", 8, 8, &err_ptr, &err_n);
  printf("  load_txt(\"non_existent.txt\"): ret=%d (expected: -1)\n", ret_err1);

  int ret_err2 = load_txt(NULL, 8, 8, &err_ptr, &err_n);
  printf("  load_txt(NULL, ...): ret=%d (expected: -1)\n", ret_err2);

  int ret_err3 = load_txt("font_data01.txt", 0, 8, &err_ptr, &err_n);
  printf("  load_txt(..., h=0, ...): ret=%d (expected: -1)\n", ret_err3);

  /* 2. load_txt_default のテスト */
  printf("\n[load_txt_default]\n");
  int (*def_patterns)[PATTERN_DEFAULT_H][PATTERN_DEFAULT_W] = NULL;
  size_t def_n = 0;
  int ret_def = load_txt_default("font_data01.txt", &def_patterns, &def_n);
  printf(
      "  load_txt_default(\"font_data01.txt\"): ret=%d, n=%zu (expected: "
      "ret=0, n=26)\n",
      ret_def, def_n);

  if (ret_def == 0 && def_n > 0) {
    printf("  letter 0 (8x8 bitmap):\n");
    for (size_t y = 0; y < PATTERN_DEFAULT_H; y++) {
      printf("    ");
      for (size_t x = 0; x < PATTERN_DEFAULT_W; x++) {
        printf("%d", def_patterns[0][y][x]);
      }
      putchar('\n');
    }
    free_memory(def_patterns);
  }

  /* 3. load_txt_to_8x8 (互換性ラッパー) のテスト */
  printf("\n[load_txt_to_8x8 (compatibility wrapper)]\n");
  int (*patterns)[PATTERN_DEFAULT_H][PATTERN_DEFAULT_W] = NULL;
  size_t n = 0;
  int ret_8x8 = load_txt_to_8x8("font_data01.txt", &patterns, &n);
  printf(
      "  load_txt_to_8x8(\"font_data01.txt\"): ret=%d, n=%zu (expected: ret=0, "
      "n=26)\n",
      ret_8x8, n);

  /* 4. free_memory のテスト */
  printf("\n[free_memory]\n");
  free_memory(patterns);
  printf("  free_memory(patterns): success\n");

  /* NULL解放の安全性 */
  free_memory(NULL);
  printf("  free_memory(NULL): success (safe no-op)\n");

  return 0;
}

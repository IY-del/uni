#include <stdio.h>
#include <stdlib.h>

#include "fontlib.c"

int main(void) {
  printf("=== 2. パターン読み込み・メモリ管理機能 ===\n");

  /* 1. static 関数 read_bits の単体テスト */
  printf("[read_bits (static)]\n");
  int* bits = NULL;
  size_t bit_len = 0;
  int ret_bits = read_bits("font_data01.txt", &bits, &bit_len);
  printf(
      "  read_bits(\"font_data01.txt\"): ret=%d, len=%zu (expected: ret=0, "
      "len=1663)\n",
      ret_bits, bit_len);
  if (ret_bits == 0) {
    printf("  first 8 bits: %d%d%d%d%d%d%d%d (expected: 00011000)\n", bits[0],
           bits[1], bits[2], bits[3], bits[4], bits[5], bits[6], bits[7]);
  }

  /* 2. static 関数 pad_to_grid の単体テスト */
  printf("\n[pad_to_grid (static)]\n");
  int* grid_out = NULL;
  size_t grid_n = 0;
  int ret_pad = pad_to_grid(bits, bit_len, 64, &grid_out, &grid_n);
  printf(
      "  pad_to_grid(..., len=%zu, cell=64): ret=%d, n=%zu (expected: ret=0, "
      "n=26)\n",
      bit_len, ret_pad, grid_n);
  free_memory(grid_out);

  /* 3. load_txt のテスト */
  printf("\n[load_txt]\n");
  int* flat = NULL;
  size_t flat_n = 0;
  int ret_flat = load_txt("font_data01.txt", 8, 8, &flat, &flat_n);
  printf(
      "  load_txt(\"font_data01.txt\", 8, 8): ret=%d, n=%zu (expected: ret=0, "
      "n=26)\n",
      ret_flat, flat_n);
  if (ret_flat == 0) {
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

  /* 4. load_txt_to_8x8 のテスト */
  printf("\n[load_txt_to_8x8]\n");
  int (*patterns)[8][8] = NULL;
  size_t n = 0;
  int ret_8x8 = load_txt_to_8x8("font_data01.txt", &patterns, &n);
  printf(
      "  load_txt_to_8x8(\"font_data01.txt\"): ret=%d, n=%zu (expected: ret=0, "
      "n=26)\n",
      ret_8x8, n);

  if (ret_8x8 == 0 && n > 0) {
    printf("  letter 0 (8x8 bitmap):\n");
    for (size_t y = 0; y < 8; y++) {
      printf("    ");
      for (size_t x = 0; x < 8; x++) {
        printf("%d", patterns[0][y][x]);
      }
      putchar('\n');
    }
  }

  /* 5. free_memory のテスト */
  printf("\n[free_memory]\n");
  free_memory(patterns);
  printf("  free_memory(patterns): success\n");

  /* NULL解放の安全性 */
  free_memory(NULL);
  printf("  free_memory(NULL): success (safe no-op)\n");

  return 0;
}

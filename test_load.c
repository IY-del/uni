#include <stdio.h>
#include <stdlib.h>

#include "fontlib.c"

/**
 * @brief static 関数 read_bits の単体検証
 * - ファイルから '0'/'1' のビット列を正確に抽出できているか
 * - 抽出ビット長および先頭文字先頭行のビット並びが仕様通りか
 */
static int* test_read_bits(size_t* out_len) {
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
  *out_len = bit_len;
  return bits;
}

/**
 * @brief static 関数 pad_to_grid の単体検証
 * - 1663 ビットのデータに対し、不足 1 ビットが 0 パディングされ
 *   64 (8x8) の倍数である 1664 要素 (26 文字分) に整形されるか
 */
static void test_pad_to_grid(int* bits, size_t bit_len) {
  printf("\n[pad_to_grid (static)]\n");
  int* grid_out = NULL;
  size_t grid_n = 0;
  int ret_pad = pad_to_grid(bits, bit_len, 64, &grid_out, &grid_n);
  printf(
      "  pad_to_grid(..., len=%zu, cell=64): ret=%d, n=%zu (expected: ret=0, "
      "n=26)\n",
      bit_len, ret_pad, grid_n);
  free_memory(grid_out);
}

/**
 * @brief load_txt 関数の正常系・異常系・境界値検証
 * - 正常系: 8x8 指定での 26 文字読み込み
 * - 異常系: 存在しないファイルパス (-1)
 * - 堅牢性: NULL ポインタ引数 (-1)
 * - 境界値: h=0 (ゼロ除算防止, -1)
 */
static void test_load_txt(void) {
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
}

/**
 * @brief load_txt_to_8x8 関数の動作検証
 * - 8x8 固定二次元配列ポインタとしての読み込み
 * - 先頭文字ビットマップの展開結果の可視化
 */
static int (*test_load_txt_to_8x8(size_t* out_n))[8][8] {
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
  *out_n = n;
  return patterns;
}

/**
 * @brief free_memory 関数の動作検証
 * - 動的確保領域の解放
 * - NULL ポインタ解放時の安全性 (safe no-op)
 */
static void test_free_memory(int (*patterns)[8][8]) {
  printf("\n[free_memory]\n");
  free_memory(patterns);
  printf("  free_memory(patterns): success\n");

  /* NULL解放の安全性 */
  free_memory(NULL);
  printf("  free_memory(NULL): success (safe no-op)\n");
}

int main(void) {
  printf("=== 2. パターン読み込み・メモリ管理機能 ===\n");

  size_t bit_len = 0;
  int* bits = test_read_bits(&bit_len);

  test_pad_to_grid(bits, bit_len);

  test_load_txt();

  size_t n = 0;
  int (*patterns)[8][8] = test_load_txt_to_8x8(&n);

  test_free_memory(patterns);

  return 0;
}

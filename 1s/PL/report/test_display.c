#include <stdio.h>

#include "fontlib.h"

/**
 * @brief printx 関数の動作検証
 * - 真値 (1, 2) に対する 'X' 出力
 * - 偽値 (0) に対する空白出力
 */
static void test_printx(void) {
  printf("[printx]\n");
  printf("  printx(1): \"");
  printx(1);
  printf("\" (expected: \"X\")\n");

  printf("  printx(0): \"");
  printx(0);
  printf("\" (expected: \" \")\n");

  printf("  printx(2): \"");
  printx(2);
  printf("\" (expected: \"X\")\n");
}

/**
 * @brief printxs 関数の動作検証
 * - 真値 (1) に対する指定文字列出力
 * - 偽値 (0) に対する空白1文字出力
 * - NULL ポインタに対する安全ガード
 */
static void test_printxs(void) {
  printf("\n[printxs]\n");
  printf("  printxs(1, \"ABC\"): \"");
  printxs(1, "ABC");
  printf("\" (expected: \"ABC\")\n");

  printf("  printxs(0, \"ABC\"): \"");
  printxs(0, "ABC");
  printf("\" (expected: \" \")\n");

  printf("  printxs(1, NULL): ");
  printxs(1, NULL);
  printf("(safe no-op)\n");
}

int main(void) {
  printf("=== 1. 基本表示機能 ===\n");
  test_printx();
  test_printxs();
  return 0;
}

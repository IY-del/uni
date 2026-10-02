#include <stdio.h>

#include "fontlib.h"

int main(void) {
  printf("=== 1. 基本表示機能 ===\n");

  /* 1. printx のテスト */
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

  /* 2. printxs のテスト */
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

  return 0;
}

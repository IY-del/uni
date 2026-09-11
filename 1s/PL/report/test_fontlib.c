#include <stdio.h>

#include "fontlib.h"

int main(void) {
  /* printx のテスト */
  printx(1);
  printx(0);
  printx(2);
  putchar('\n');

  /* printxs のテスト */
  printxs(1, "ABC");
  putchar('\n');

  printxs(0, "ABC");
  putchar('\n');

  return 0;
}

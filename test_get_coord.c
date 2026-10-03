#include <stdio.h>

#include "fontlib.c"

/**
 * @brief static 関数 rotate_coord の直接単体検証
 * - 各回転角 (ROTATE_0, ROTATE_90, ROTATE_180, ROTATE_270) における座標写像の検証
 * - NULL ポインタ入力時の安全ガード検証
 */
static void test_rotate_coord(PatternView* view) {
  printf("[rotate_coord (static)]\n");
  view->rotation = ROTATE_0;
  PatternCoord r0 = rotate_coord(view, 2, 1);
  printf("  rotate_coord(ROTATE_0, 2, 1): (%zu,%zu) (expected: (2,1))\n", r0.x,
         r0.y);

  view->rotation = ROTATE_90;
  PatternCoord r90 = rotate_coord(view, 2, 1);
  printf("  rotate_coord(ROTATE_90, 2, 1): (%zu,%zu) (expected: (1,1))\n", r90.x,
         r90.y);

  view->rotation = ROTATE_180;
  PatternCoord r180 = rotate_coord(view, 2, 1);
  printf("  rotate_coord(ROTATE_180, 2, 1): (%zu,%zu) (expected: (5,2))\n",
         r180.x, r180.y);

  view->rotation = ROTATE_270;
  PatternCoord r270 = rotate_coord(view, 2, 1);
  printf("  rotate_coord(ROTATE_270, 2, 1): (%zu,%zu) (expected: (6,2))\n",
         r270.x, r270.y);

  PatternCoord r_null = rotate_coord(NULL, 0, 0);
  printf("  rotate_coord(NULL, 0, 0): (%zu,%zu) (expected: (0,0))\n", r_null.x,
         r_null.y);
}

/**
 * @brief pattern_scaled_size 関数の動作検証
 * - 非正方形パターン (w=8, h=4) における回転時の幅・高さ転置検証
 * - 拡大スケーリング (scale_w=3, scale_h=2) の乗算検証
 * - 回転と拡大の組み合わせ検証
 * - NULL ポインタ入力時の安全ガード検証
 */
static void test_pattern_scaled_size(PatternView* view) {
  printf("\n[pattern_scaled_size]\n");
  view->rotation = ROTATE_0;
  view->scale_w = 1;
  view->scale_h = 1;
  PatternSize size = pattern_scaled_size(view);
  printf("  rotate0:               size=(w=%zu,h=%zu) (expected: w=8,h=4)\n",
         size.width, size.height);

  view->rotation = ROTATE_90;
  size = pattern_scaled_size(view);
  printf("  rotate90:              size=(w=%zu,h=%zu) (expected: w=4,h=8)\n",
         size.width, size.height);

  view->rotation = ROTATE_180;
  size = pattern_scaled_size(view);
  printf("  rotate180:             size=(w=%zu,h=%zu) (expected: w=8,h=4)\n",
         size.width, size.height);

  view->rotation = ROTATE_270;
  size = pattern_scaled_size(view);
  printf("  rotate270:             size=(w=%zu,h=%zu) (expected: w=4,h=8)\n",
         size.width, size.height);

  view->rotation = ROTATE_0;
  view->scale_w = 3;
  view->scale_h = 2;
  size = pattern_scaled_size(view);
  printf("  scale_w3_h2:           size=(w=%zu,h=%zu) (expected: w=24,h=8)\n",
         size.width, size.height);

  view->rotation = ROTATE_90;
  size = pattern_scaled_size(view);
  printf("  rotate90_scale_w3_h2:  size=(w=%zu,h=%zu) (expected: w=12,h=16)\n",
         size.width, size.height);

  PatternSize null_size = pattern_scaled_size(NULL);
  printf("  pattern_scaled_size(NULL): (w=%zu,h=%zu) (expected: (0,0))\n",
         null_size.width, null_size.height);
}

/**
 * @brief resolve_coord 関数の動作検証
 * - 原点 (0,0) および境界対角点における各回転方向での座標逆写像検証
 * - スケーリング時の除算逆写像検証
 * - NULL ポインタ入力時の安全ガード検証
 */
static void test_resolve_coord(PatternView* view) {
  printf("\n[resolve_coord]\n");
  view->scale_w = 1;
  view->scale_h = 1;

  view->rotation = ROTATE_0;
  PatternCoord src = resolve_coord(view, 0, 0);
  printf("  rotate0:               resolve(0,0) -> (%zu,%zu) (expected: (0,0))\n",
         src.x, src.y);
  src = resolve_coord(view, 7, 3);
  printf(
      "  rotate0 corner:        resolve(7,3) -> (%zu,%zu) (expected: (7,3))\n",
      src.x, src.y);

  view->rotation = ROTATE_90;
  src = resolve_coord(view, 0, 0);
  printf("  rotate90:              resolve(0,0) -> (%zu,%zu) (expected: (0,3))\n",
         src.x, src.y);
  src = resolve_coord(view, 3, 7);
  printf(
      "  rotate90 corner:       resolve(3,7) -> (%zu,%zu) (expected: (7,0))\n",
      src.x, src.y);

  view->rotation = ROTATE_180;
  src = resolve_coord(view, 0, 0);
  printf(
      "  rotate180:             resolve(0,0) -> (%zu,%zu) (expected: (7,3))\n",
      src.x, src.y);
  src = resolve_coord(view, 7, 3);
  printf(
      "  rotate180 corner:      resolve(7,3) -> (%zu,%zu) (expected: (0,0))\n",
      src.x, src.y);

  view->rotation = ROTATE_270;
  src = resolve_coord(view, 0, 0);
  printf("  rotate270:             resolve(0,0) -> (%zu,%zu) (expected: (7,0))\n",
         src.x, src.y);
  src = resolve_coord(view, 3, 7);
  printf(
      "  rotate270 corner:      resolve(3,7) -> (%zu,%zu) (expected: (0,3))\n",
      src.x, src.y);

  view->rotation = ROTATE_0;
  view->scale_w = 3;
  view->scale_h = 2;
  src = resolve_coord(view, 0, 0);
  printf("  scale_w3_h2:           resolve(0,0) -> (%zu,%zu) (expected: (0,0))\n",
         src.x, src.y);

  view->rotation = ROTATE_90;
  src = resolve_coord(view, 0, 0);
  printf(
      "  rotate90_scale_w3_h2:  resolve(0,0) -> (%zu,%zu) (expected: (0,3))\n",
      src.x, src.y);

  PatternCoord null_coord = resolve_coord(NULL, 0, 0);
  printf("  resolve_coord(NULL, 0, 0): (%zu,%zu) (expected: (0,0))\n",
         null_coord.x, null_coord.y);
}

int main(void) {
  printf("=== 3. 座標変換・サイズ計算機能 ===\n");

  int dummy[1][8][8] = {{{0}}};
  PatternView view = new_pattern_view(1, dummy);

  /* 非正方形 (w=8, h=4) にして軸の入れ替わりを可視化 */
  view.w = 8;
  view.h = 4;

  test_rotate_coord(&view);
  test_pattern_scaled_size(&view);
  test_resolve_coord(&view);

  return 0;
}

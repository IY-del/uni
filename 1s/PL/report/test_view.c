#include <stdio.h>

#include "fontlib.h"

/**
 * @brief new_pattern_view 関数の初期化検証
 * - 標準ビュー（等倍 scale=1, 正立 ROTATE_90）のパラメータ設定確認
 */
static PatternView test_new_pattern_view(int patterns[1][8][8]) {
  printf("[new_pattern_view]\n");
  PatternView view = new_pattern_view(1, patterns);
  printf("  normal: h=%zu w=%zu scale_w=%u scale_h=%u rotation=%d\n", view.h,
         view.w, view.scale_w, view.scale_h, view.rotation);
  return view;
}

/**
 * @brief new_pattern_view_doublescale 関数の初期化検証
 * - 2倍拡大ビュー（scale=2, 正立 ROTATE_90）のパラメータ設定確認
 */
static PatternView test_new_pattern_view_doublescale(int patterns[1][8][8]) {
  printf("\n[new_pattern_view_doublescale]\n");
  PatternView view2x = new_pattern_view_doublescale(1, patterns);
  printf("  double: h=%zu w=%zu scale_w=%u scale_h=%u rotation=%d\n", view2x.h,
         view2x.w, view2x.scale_w, view2x.scale_h, view2x.rotation);
  return view2x;
}

/**
 * @brief print_as_pattern 関数の描画検証
 * - 等倍ビューでの文字パターン描画
 * - 2倍拡大ビューでの文字パターン描画
 */
static void test_print_as_pattern(const PatternView* view,
                                  const PatternView* view2x) {
  printf("\n[print_as_pattern (normal)]\n");
  print_as_pattern(view);

  printf("[print_as_pattern (double scale)]\n");
  print_as_pattern(view2x);
}

/**
 * @brief print_as_pattern 関数の NULL 安全性検証
 * - NULL ポインタ入力時にクラッシュせず安全に復帰すること (safe no-op)
 */
static void test_print_as_pattern_null_safety(void) {
  printf("[NULL Safety]\n");
  print_as_pattern(NULL);
  printf("  print_as_pattern(NULL): safe no-op\n");
}

int main(void) {
  printf("=== 5. パターン描画制御機能 ===\n");

  int patterns[1][8][8] = {{{0, 0, 1, 1, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0, 0, 0, 0},
                            {1, 1, 1, 1, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0}}};

  PatternView view = test_new_pattern_view(patterns);
  PatternView view2x = test_new_pattern_view_doublescale(patterns);

  test_print_as_pattern(&view, &view2x);
  test_print_as_pattern_null_safety();

  return 0;
}

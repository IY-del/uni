#include <stdio.h>

#include "fontlib.h"

/**
 * @brief new_iter 関数の初期化状態検証
 * - 出力寸法 (out_w, out_h) および走査座標 (x, y, ch) の初期値が 0 であること
 */
static void test_new_iter(const PatternView* view) {
  printf("[new_iter]\n");
  PatternIterator it = new_iter(view);
  printf(
      "  new_iter: out_w=%zu out_h=%zu x=%zu y=%zu ch=%zu (expected: out_w=8 "
      "out_h=8 x=0 y=0 ch=0)\n\n",
      it.out_w, it.out_h, it.x, it.y, it.ch);
}

/**
 * @brief pattern_iterator_next 関数の走査シーケンス検証（行優先）
 * - 1文字完結型での CELL / NEWLINE 発行順序
 * - 全文字走査完了時の PATTERN_EVENT_END 発行確認
 */
static void test_pattern_iterator_next(const PatternView* view) {
  printf("[pattern_iterator_next (行優先走査)]\n");
  PatternIterator it = new_iter(view);
  PatternStep step;

  printf("pattern_iterator_next\n");
  while (pattern_iterator_next(&it, &step) != PATTERN_EVENT_END) {
    if (step.kind == PATTERN_EVENT_CELL) {
      printf("CELL ch=%zu src=(%zu,%zu)\n", step.ch, step.src_x, step.src_y);
    } else if (step.kind == PATTERN_EVENT_NEWLINE) {
      printf("NEWLINE\n");
    }
  }
  printf("END\n");
}

/**
 * @brief pattern_iterator_next_h 関数の走査シーケンス検証（水平優先）
 * - 全文字並行型での CELL / NEWLINE 発行順序
 * - 全行走査完了時の PATTERN_EVENT_END 発行確認
 */
static void test_pattern_iterator_next_h(const PatternView* view) {
  printf("\n[pattern_iterator_next_h (水平優先走査)]\n");
  PatternIterator it = new_iter(view);
  PatternStep step;

  printf("pattern_iterator_next_h\n");
  while (pattern_iterator_next_h(&it, &step) != PATTERN_EVENT_END) {
    if (step.kind == PATTERN_EVENT_CELL) {
      printf("CELL ch=%zu src=(%zu,%zu)\n", step.ch, step.src_x, step.src_y);
    } else if (step.kind == PATTERN_EVENT_NEWLINE) {
      printf("NEWLINE\n");
    }
  }
  printf("END\n");
}

/**
 * @brief イテレータ関数の NULL 安全性検証
 * - NULL ポインタ入力時にクラッシュせず即座に PATTERN_EVENT_END を返すか
 */
static void test_iterator_null_safety(void) {
  printf("\n[NULL Safety]\n");
  PatternEvent ev = pattern_iterator_next(NULL, NULL);
  printf("  pattern_iterator_next(NULL, NULL): %d (expected: %d)\n", ev,
         PATTERN_EVENT_END);
}

int main(void) {
  printf("=== 4. パターン走査機能 ===\n");

  int dummy[2][8][8] = {{{0}}};
  PatternView view = new_pattern_view(2, dummy);

  test_new_iter(&view);
  test_pattern_iterator_next(&view);
  test_pattern_iterator_next_h(&view);
  test_iterator_null_safety();

  return 0;
}

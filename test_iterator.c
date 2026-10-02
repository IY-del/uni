#include <stdio.h>

#include "fontlib.h"

static void run_next(const PatternView* view) {
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

static void run_next_h(const PatternView* view) {
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

int main(void) {
  printf("=== 4. パターン走査機能 ===\n");

  int dummy[2][8][8] = {{{0}}};
  PatternView view = new_pattern_view(2, dummy);

  printf("[new_iter]\n");
  PatternIterator it = new_iter(&view);
  printf("  new_iter: out_w=%zu out_h=%zu x=%zu y=%zu ch=%zu (expected: out_w=8 out_h=8 x=0 y=0 ch=0)\n\n",
         it.out_w, it.out_h, it.x, it.y, it.ch);

  printf("[pattern_iterator_next (行優先走査)]\n");
  run_next(&view);

  printf("\n[pattern_iterator_next_h (水平優先走査)]\n");
  run_next_h(&view);

  /* NULL安全テスト */
  printf("\n[NULL Safety]\n");
  PatternEvent ev1 = pattern_iterator_next(NULL, NULL);
  printf("  pattern_iterator_next(NULL, NULL): %d (expected: %d)\n", ev1, PATTERN_EVENT_END);

  return 0;
}

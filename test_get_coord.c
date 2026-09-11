#include <stdio.h>

#include "fontlib.h"

static void report_coord(const char* label, const PatternView* view, size_t ox,
                         size_t oy) {
  PatternSize size = pattern_scaled_size(view);
  PatternCoord src = resolve_coord(view, ox, oy);
  printf("%-22s size=(w=%zu,h=%zu) resolve(%zu,%zu) -> (%zu,%zu)\n", label,
         size.width, size.height, ox, oy, src.x, src.y);
}

int main(void) {
  int dummy[1][8][8] = {{{0}}};
  PatternView view = new_pattern_view(1, dummy);

  /* 非正方形にして w/h の入れ替わりを可視化する */
  view.w = 8;
  view.h = 4;

  /* 1. 回転なし（基準確認） */
  view.rotation = ROTATE_0;
  report_coord("rotate0", &view, 0, 0);
  report_coord("rotate0 corner", &view, 7, 3);

  /* 2. 90度回転（w/hが入れ替わるはず） */
  view.rotation = ROTATE_90;
  report_coord("rotate90", &view, 0, 0);
  report_coord("rotate90 corner", &view, 3, 7);

  /* 3. 180度回転 */
  view.rotation = ROTATE_180;
  report_coord("rotate180", &view, 0, 0);
  report_coord("rotate180 corner", &view, 7, 3);

  /* 4. 270度回転 */
  view.rotation = ROTATE_270;
  report_coord("rotate270", &view, 0, 0);
  report_coord("rotate270 corner", &view, 3, 7);

  /* 5. 非対称スケール（scale_wとscale_hを別々の値にして独立性を確認） */
  view.rotation = ROTATE_0;
  view.scale_w = 3;
  view.scale_h = 2;
  report_coord("scale_w3_h2", &view, 0, 0);

  /* 6. 回転90度 + 非対称スケールの組み合わせ */
  view.rotation = ROTATE_90;
  report_coord("rotate90_scale_w3_h2", &view, 0, 0);

  return 0;
}

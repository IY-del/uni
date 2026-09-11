#include <stdio.h>

#include "fontlib.h"

int main(void) {
  int dummy[1][8][8] = {{{0}}};
  PatternView view = new_pattern_view(1, dummy);
  PatternSize size;
  PatternCoord src;

  /* 非正方形にして w/h の入れ替わりを可視化する */
  view.w = 8;
  view.h = 4;

  /* 1. pattern_scaled_size の検証 */
  view.rotation = ROTATE_0;
  size = pattern_scaled_size(&view);
  printf("%-22s size=(w=%zu,h=%zu)\n", "rotate0:", size.width, size.height);

  view.rotation = ROTATE_90;
  size = pattern_scaled_size(&view);
  printf("%-22s size=(w=%zu,h=%zu)\n", "rotate90:", size.width, size.height);

  view.rotation = ROTATE_180;
  size = pattern_scaled_size(&view);
  printf("%-22s size=(w=%zu,h=%zu)\n", "rotate180:", size.width, size.height);

  view.rotation = ROTATE_270;
  size = pattern_scaled_size(&view);
  printf("%-22s size=(w=%zu,h=%zu)\n", "rotate270:", size.width, size.height);

  view.rotation = ROTATE_0;
  view.scale_w = 3;
  view.scale_h = 2;
  size = pattern_scaled_size(&view);
  printf("%-22s size=(w=%zu,h=%zu)\n", "scale_w3_h2:", size.width, size.height);

  view.rotation = ROTATE_90;
  size = pattern_scaled_size(&view);
  printf("%-22s size=(w=%zu,h=%zu)\n", "rotate90_scale_w3_h2:", size.width,
         size.height);

  /* 2. resolve_coord の検証 */
  view.scale_w = 1;
  view.scale_h = 1;

  view.rotation = ROTATE_0;
  src = resolve_coord(&view, 0, 0);
  printf("%-22s resolve(0,0) -> (%zu,%zu)\n", "rotate0:", src.x, src.y);
  src = resolve_coord(&view, 7, 3);
  printf("%-22s resolve(7,3) -> (%zu,%zu)\n", "rotate0 corner:", src.x, src.y);

  view.rotation = ROTATE_90;
  src = resolve_coord(&view, 0, 0);
  printf("%-22s resolve(0,0) -> (%zu,%zu)\n", "rotate90:", src.x, src.y);
  src = resolve_coord(&view, 3, 7);
  printf("%-22s resolve(3,7) -> (%zu,%zu)\n", "rotate90 corner:", src.x, src.y);

  view.rotation = ROTATE_180;
  src = resolve_coord(&view, 0, 0);
  printf("%-22s resolve(0,0) -> (%zu,%zu)\n", "rotate180:", src.x, src.y);
  src = resolve_coord(&view, 7, 3);
  printf("%-22s resolve(7,3) -> (%zu,%zu)\n", "rotate180 corner:", src.x, src.y);

  view.rotation = ROTATE_270;
  src = resolve_coord(&view, 0, 0);
  printf("%-22s resolve(0,0) -> (%zu,%zu)\n", "rotate270:", src.x, src.y);
  src = resolve_coord(&view, 3, 7);
  printf("%-22s resolve(3,7) -> (%zu,%zu)\n", "rotate270 corner:", src.x, src.y);

  view.rotation = ROTATE_0;
  view.scale_w = 3;
  view.scale_h = 2;
  src = resolve_coord(&view, 0, 0);
  printf("%-22s resolve(0,0) -> (%zu,%zu)\n", "scale_w3_h2:", src.x, src.y);

  view.rotation = ROTATE_90;
  src = resolve_coord(&view, 0, 0);
  printf("%-22s resolve(0,0) -> (%zu,%zu)\n", "rotate90_scale_w3_h2:", src.x,
         src.y);

  return 0;
}

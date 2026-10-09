#include "fontlib.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printx(int flag) { printf("%s", flag ? "X" : " "); }

void printxs(int flag, const char str[]) {
  if (!str) return;
  printf("%s", flag ? str : " ");
}

static PatternCoord rotate_coord(const PatternView* view, size_t x, size_t y) {
  PatternCoord coord = {0, 0};
  if (!view) return coord;
  switch (view->rotation) {
    case ROTATE_0:
      coord.x = x;
      coord.y = y;
      break;
    case ROTATE_90:
      coord.x = y;
      coord.y = view->h > x ? view->h - 1 - x : 0;
      break;
    case ROTATE_180:
      coord.x = view->w > x ? view->w - 1 - x : 0;
      coord.y = view->h > y ? view->h - 1 - y : 0;
      break;
    case ROTATE_270:
      coord.x = view->w > y ? view->w - 1 - y : 0;
      coord.y = x;
      break;
  }

  return coord;
}

PatternSize pattern_scaled_size(const PatternView* view) {
  PatternSize size = {0, 0};
  if (!view) return size;

  if (view->rotation == ROTATE_90 || view->rotation == ROTATE_270) {
    size.width = view->h * view->scale_w;
    size.height = view->w * view->scale_h;
  } else {
    size.width = view->w * view->scale_w;
    size.height = view->h * view->scale_h;
  }

  return size;
}

PatternCoord resolve_coord(const PatternView* view, size_t out_x,
                           size_t out_y) {
  PatternCoord coord = {0, 0};
  if (!view || view->scale_w == 0 || view->scale_h == 0) return coord;
  size_t logical_x = out_x / view->scale_w;
  size_t logical_y = out_y / view->scale_h;
  return rotate_coord(view, logical_x, logical_y);
}

PatternIterator new_iter(const PatternView* view) {
  if (!view) {
    PatternIterator it = {0};
    return it;
  }
  PatternSize size = pattern_scaled_size(view);

  PatternIterator it = {
      .view = view,
      .out_w = size.width,
      .out_h = size.height,
      .x = 0,
      .y = 0,
      .ch = 0,
  };

  return it;
}

PatternEvent pattern_iterator_next(PatternIterator* it, PatternStep* step) {
  if (!it || !step || !it->view) return PATTERN_EVENT_END;

  if (it->x >= it->out_w) {
    it->x = 0;
    it->y++;
    step->kind = PATTERN_EVENT_NEWLINE;
    if (it->y >= it->out_h) {
      it->y = 0;
      it->ch++;
    }

    if (it->ch >= it->view->count) {
      step->kind = PATTERN_EVENT_END;
    }
    return step->kind;
  }
  PatternCoord src = resolve_coord(it->view, it->x, it->y);
  step->ch = it->ch;
  step->src_x = src.x;
  step->src_y = src.y;
  step->kind = PATTERN_EVENT_CELL;
  it->x++;
  return step->kind;
}

PatternEvent pattern_iterator_next_h(PatternIterator* it, PatternStep* step) {
  if (!it || !step || !it->view) return PATTERN_EVENT_END;

  if (it->x >= it->out_w) {
    it->x = 0;
    it->ch++;
  }
  if (it->ch >= it->view->count) {
    it->ch = 0;
    it->y++;
    step->kind = PATTERN_EVENT_NEWLINE;
    if (it->y >= it->out_h) {
      step->kind = PATTERN_EVENT_END;
    }
    return step->kind;
  }
  PatternCoord src = resolve_coord(it->view, it->x, it->y);
  step->ch = it->ch;
  step->src_x = src.x;
  step->src_y = src.y;
  step->kind = PATTERN_EVENT_CELL;
  it->x++;
  return step->kind;
}

PatternView new_pattern_view(size_t count, void* pattern) {
  PatternView view = {.count = count,
                      .h = 8,
                      .w = 8,
                      .pattern = pattern,
                      .string = "X",
                      .scale_w = 1,
                      .scale_h = 1,
                      .rotation = ROTATE_90};

  return view;
}

PatternView new_pattern_view_doublescale(size_t count, void* pattern) {
  PatternView view = new_pattern_view(count, pattern);
  view.scale_w = 2;
  view.scale_h = 2;
  return view;
}

void print_as_pattern(const PatternView* view) {
  if (!view || !view->pattern) return;
  int (*pattern)[view->h][view->w] = view->pattern;
  PatternIterator it = new_iter(view);
  PatternStep step;
  while (pattern_iterator_next(&it, &step)) {
    switch (step.kind) {
      case PATTERN_EVENT_CELL:
        printxs(pattern[step.ch][step.src_y][step.src_x], view->string);
        break;
      case PATTERN_EVENT_NEWLINE:
        putchar('\n');
        break;
      case PATTERN_EVENT_END:
        break;
    }
  }
  putchar('\n');
}

static int read_bits(const char* filename, int** out_buf, size_t* out_len) {
  FILE* fp = fopen(filename, "r");
  if (!fp) return -1;

  size_t cap = 256, len = 0;
  int* buf = malloc(cap * sizeof(int));
  if (!buf) {
    fclose(fp);
    return -2;
  }

  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '0' || ch == '1') {
      if (len == cap) {
        cap *= 2;
        int* tmp = realloc(buf, cap * sizeof(int));
        if (!tmp) {
          free(buf);
          fclose(fp);
          return -2;
        }
        buf = tmp;
      }
      buf[len++] = ch - '0';
    }
  }
  fclose(fp);

  *out_buf = buf;
  *out_len = len;
  return 0;
}

static int pad_to_grid(int* buf, size_t len, size_t cell, int** out,
                       size_t* out_n) {
  size_t n = (len + cell - 1) / cell;
  size_t total = n * cell;

  if (total == 0) {
    free(buf);
    *out = NULL;
    *out_n = 0;
    return 0;
  }

  int* res = realloc(buf, total * sizeof(int));
  if (!res) {
    free(buf);
    return -2;
  }

  if (total > len) {
    memset(&res[len], 0, (total - len) * sizeof(int));
  }

  *out = res;
  *out_n = n;
  return 0;
}

int load_txt(const char* filename, size_t h, size_t w, int** out,
             size_t* out_n) {
  if (!filename || !out || !out_n || h == 0 || w == 0) return -1;
  *out = NULL;
  *out_n = 0;

  int* raw = NULL;
  size_t len = 0;
  int ret = read_bits(filename, &raw, &len);
  if (ret != 0) {
    return ret;
  }

  return pad_to_grid(raw, len, h * w, out, out_n);
}

int load_txt_to_8x8(const char filename[], int (**out)[8][8], size_t* out_n) {
  if (!filename || !out || !out_n) return -1;
  int* flat = NULL;
  int ret = load_txt(filename, 8, 8, &flat, out_n);
  if (ret != 0) {
    return ret;
  }
  *out = (int (*)[8][8])flat;
  return 0;
}

void free_memory(void* arr) {
  if (arr) free(arr);
}

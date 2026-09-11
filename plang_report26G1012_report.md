# パターン印刷プログラム結果報告書

- 科目名: プログラミング言語
- 学籍番号: 26G1012
- 氏名: 石井陽泰

---

## 目次

- [パターン印刷プログラム結果報告書](#パターン印刷プログラム結果報告書)
   - [目次](#目次)
   - [1. 報告概要](#1-報告概要)
   - [2. fontlib 機能別テストプログラムの実行結果](#2-fontlib-機能別テストプログラムの実行結果)
      - [2.1 基本表示機能の検証 (test\_fontlib.c)](#21-基本表示機能の検証-test_fontlibc)
         - [検証用プログラム](#検証用プログラム)
         - [printx の実行結果と考察](#printx-の実行結果と考察)
            - [確認する機能](#確認する機能)
            - [実行結果](#実行結果)
            - [考察](#考察)
         - [printxs の実行結果と考察](#printxs-の実行結果と考察)
            - [確認する機能](#確認する機能-1)
            - [実行結果](#実行結果-1)
            - [考察](#考察-1)
      - [2.2 パターン読み込み機能の検証 (test\_get\_pattern.c)](#22-パターン読み込み機能の検証-test_get_patternc)
         - [検証用プログラム](#検証用プログラム-1)
         - [load\_txt\_to\_8x8 の実行結果と考察](#load_txt_to_8x8-の実行結果と考察)
            - [確認する機能](#確認する機能-2)
            - [実行結果](#実行結果-2)
            - [考察](#考察-2)
         - [free\_memory の実行結果と考察](#free_memory-の実行結果と考察)
            - [確認する機能](#確認する機能-3)
            - [実行結果](#実行結果-3)
            - [考察](#考察-3)
      - [2.3 座標変換・サイズ計算機能の検証 (test\_get\_coord.c)](#23-座標変換サイズ計算機能の検証-test_get_coordc)
         - [検証用プログラム](#検証用プログラム-2)
         - [pattern\_scaled\_size の実行結果と考察](#pattern_scaled_size-の実行結果と考察)
            - [確認する機能](#確認する機能-4)
            - [実行結果](#実行結果-4)
            - [考察](#考察-4)
         - [resolve\_coord の実行結果と考察](#resolve_coord-の実行結果と考察)
            - [確認する機能](#確認する機能-5)
            - [実行結果](#実行結果-5)
            - [考察](#考察-5)
      - [2.4 パターン走査機能の検証 (test\_iterator.c)](#24-パターン走査機能の検証-test_iteratorc)
         - [検証用プログラム](#検証用プログラム-3)
         - [pattern\_iterator\_next (行優先走査) の実行結果と考察](#pattern_iterator_next-行優先走査-の実行結果と考察)
            - [確認する機能](#確認する機能-6)
            - [実行結果（抜粋）](#実行結果抜粋)
            - [考察](#考察-6)
         - [pattern\_iterator\_next\_h (水平優先走査) の実行結果と考察](#pattern_iterator_next_h-水平優先走査-の実行結果と考察)
            - [確認する機能](#確認する機能-7)
            - [実行結果（抜粋）](#実行結果抜粋-1)
            - [考察](#考察-7)
      - [2.5 パターン描画制御機能の検証 (test\_print\_pattern.c)](#25-パターン描画制御機能の検証-test_print_patternc)
         - [検証用プログラム](#検証用プログラム-4)
         - [new\_pattern\_view / new\_pattern\_view\_doublescale の実行結果と考察](#new_pattern_view--new_pattern_view_doublescale-の実行結果と考察)
            - [確認する機能](#確認する機能-8)
            - [実行結果](#実行結果-6)
            - [考察](#考察-8)
         - [print\_as\_pattern の実行結果と考察](#print_as_pattern-の実行結果と考察)
            - [確認する機能](#確認する機能-9)
            - [実行結果](#実行結果-7)
            - [考察](#考察-9)
   - [3. 統合テストプログラム (main.c) の実行結果](#3-統合テストプログラム-mainc-の実行結果)
      - [3.1 パイプライン統合動作の検証](#31-パイプライン統合動作の検証)
      - [3.2 実行結果と考察](#32-実行結果と考察)
         - [確認する機能](#確認する機能-10)
         - [実行結果（抜粋）](#実行結果抜粋-2)
         - [考察](#考察-10)
   - [4. 総合考察](#4-総合考察)

---

## 1. 報告概要

本報告書では，[パターン印刷プログラム仕様書](plang_report26G1012_spec.md) に基づいて実装された共通ライブラリ `fontlib` の各機能群（基本表示，パターン読み込み，座標変換・寸法計算，走査イテレータ，描画制御）および統合テストプログラム `main.c` について，機能単位のサンプル検証プログラムを作成・実行し，各機能が仕様どおりに動作しているかを確認した結果を報告する．

各テストプログラムは，可読性およびポータビリティを担保するため，Cソースコード内でのマクロ定義（`#define`）やグローバル変数の使用を一切行わず，関数ローカル変数および公開ヘッダー `fontlib.h` のシグネチャのみを用いて記述されている．また，ディレクトリ構造をフラットに保つため，すべてのテストプログラムはライブラリ本体と同じカレントディレクトリ上に配置している．

---

## 2. fontlib 機能別テストプログラムの実行結果

### 2.1 基本表示機能の検証 (test_fontlib.c)

#### 検証用プログラム

1セルの文字描画を行う `printx` および文字列描画を行う `printxs` を検証するためのテストプログラムである．

```c
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
```

コンパイルおよび実行コマンド：

```sh
clang test_fontlib.c fontlib.c -o test_fontlib
./test_fontlib
```

#### printx の実行結果と考察

##### 確認する機能

`flag` が 0 のときスペース，0 以外のとき `"X"` を1文字表示する機能を確認する．

##### 実行結果

```text
X X
```

##### 考察

仕様書では，1回目（`flag=1`）と3回目（`flag=2`）の呼び出しで `"X"` が表示され，2回目（`flag=0`）ではスペースが表示されると規定されている．実行結果でも同じ並びで `"X"` とスペースが出力されているため，`printx` は `flag` の真偽値に応じて正しく表示内容を切り替えているといえる．

#### printxs の実行結果と考察

##### 確認する機能

`flag` が 0 のときスペース1文字，0 以外のとき第2引数 `str` の文字列を表示する機能を確認する．

##### 実行結果

```text
ABC

```

##### 考察

仕様書では，`flag` が 0 以外の場合は引数 `str` の内容が表示され，`flag` が 0 の場合は `str` の内容に関係なくスペース1文字だけが表示されるとされている．実行結果では，`flag=1` の呼び出しで `"ABC"` が表示され，`flag=0` の呼び出しではスペースのみが出力されている．したがって，`printxs` は仕様どおりに動作していることが確認できる．

---

### 2.2 パターン読み込み機能の検証 (test_get_pattern.c)

#### 検証用プログラム

テキストファイルから 8×8 のフォントパターン配列を動的に読み込む `load_txt_to_8x8` と，確保したメモリを解放する `free_memory` を検証するためのテストプログラムである．

```c
#include <stdio.h>

#include "fontlib.h"

int main(void) {
  int (*patterns)[8][8];
  size_t n;

  /* load_txt_to_8x8 のテスト */
  if (load_txt_to_8x8("font_data01.txt", &patterns, &n) != 0) {
    fprintf(stderr, "load_txt_to_8x8 failed (font_data01.txt)\n");
    return 1;
  }

  printf("n = %zu\n", n);

  if (n > 0) {
    for (size_t y = 0; y < 8; y++) {
      for (size_t x = 0; x < 8; x++) {
        printf("%d", patterns[0][y][x]);
      }
      putchar('\n');
    }
  }

  /* free_memory のテスト */
  free_memory(patterns);

  return 0;
}
```

コンパイルおよび実行コマンド：

```sh
clang test_get_pattern.c fontlib.c -o test_get_pattern
./test_get_pattern
```

#### load_txt_to_8x8 の実行結果と考察

##### 確認する機能

テキストファイル中の 0/1 列から，8×8 のパターン配列を複数文字分生成する機能を確認する．

##### 実行結果

```text
n = 26
00011000
00100100
01000010
01000010
01111110
01000010
01000010
00000000
```

##### 考察

仕様書では，0/1 列を 8×8 単位でまとめて複数文字分のパターン配列を生成するとされている．実行結果では，読み込まれた文字数が `n = 26` となっており，`font_data01.txt` から 26 文字分の 8×8 パターンが生成されたことが分かる．また，先頭文字のパターンとして 8 行 × 8 列のビット列が正しく出力されているため，`load_txt_to_8x8` はファイルを正しく解釈し，2次元配列ポインタとして格納できているといえる．

#### free_memory の実行結果と考察

##### 確認する機能

`load_txt_to_8x8` で確保した動的メモリ領域を，`void *` 型で受け取り正常に解放できる機能を確認する．

##### 実行結果

プログラムがセグメンテーション違反などの異常終了を起こさず，終了コード 0 で正常終了した．

##### 考察

`free_memory` は `void *` で引数を受け取るため，`int (*)[8][8]` 型のポインタでも明示的なキャストなしで渡すことができる．実行時に異常終了が発生せず，動的メモリを正常に解放できているため，仕様どおりに機能していることが確認できた．

---

### 2.3 座標変換・サイズ計算機能の検証 (test_get_coord.c)

#### 検証用プログラム

各回転角度およびスケール倍率における出力サイズの計算（`pattern_scaled_size`）と，出力座標から元パターン座標への逆写像（`resolve_coord`）をまとめて検証するテストプログラムである．

```c
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
```

コンパイルおよび実行コマンド：

```sh
clang test_get_coord.c fontlib.c -o test_get_coord
./test_get_coord
```

#### pattern_scaled_size の実行結果と考察

##### 確認する機能

回転指定（0°, 90°, 180°, 270°）とスケール倍率に応じて，出力幅・高さが正しく計算される機能を確認する．

##### 実行結果

```text
rotate0:               size=(w=8,h=4)
rotate90:              size=(w=4,h=8)
rotate180:             size=(w=8,h=4)
rotate270:             size=(w=4,h=8)
scale_w3_h2:           size=(w=24,h=8)
rotate90_scale_w3_h2:  size=(w=12,h=16)
```

##### 考察

元パターンを `w=8, h=4` に設定して検証した結果，`rotate0` では `size=(w=8,h=4)`，`rotate90` では `size=(w=4,h=8)` となっており，90度回転によって幅と高さの軸が入れ替わっていることを確認できた．また，`rotate180` では再び `size=(w=8,h=4)` となり，0度・180度では元の軸，90度・270度では入れ替わった軸を用いるという仕様どおりの計算が行われている．

さらに，`scale_w=3, scale_h=2` を設定した場合（`scale_w3_h2`）には，幅と高さがそれぞれ別の倍率で拡大され `size=(w=24,h=8)` となっている．したがって，`pattern_scaled_size` は回転による軸の切り替えと独立したスケーリングを正しく組み合わせて出力サイズを算出できているといえる．

#### resolve_coord の実行結果と考察

##### 確認する機能

出力座標 `(out_x, out_y)` から，スケール倍率と回転指定を考慮して，元パターン上の座標 `(src_x, src_y)` を正しく逆変換する機能を確認する．

##### 実行結果

```text
rotate0:               resolve(0,0) -> (0,0)
rotate0 corner:        resolve(7,3) -> (7,3)
rotate90:              resolve(0,0) -> (0,3)
rotate90 corner:       resolve(3,7) -> (7,0)
rotate180:             resolve(0,0) -> (7,3)
rotate180 corner:      resolve(7,3) -> (0,0)
rotate270:             resolve(0,0) -> (7,0)
rotate270 corner:      resolve(3,7) -> (0,3)
scale_w3_h2:           resolve(0,0) -> (0,0)
rotate90_scale_w3_h2:  resolve(0,0) -> (0,3)
```

##### 考察

回転なしの `rotate0` では，出力座標が元パターン座標にそのまま対応している．`rotate90` では `resolve(0,0) -> (0,3)`，`resolve(3,7) -> (7,0)` となり，時計回り90度の回転が正しく反映されている．`rotate180` では上下左右が完全に反転し，`rotate270` でも期待どおりの座標が得られている．

また，拡大倍率を含む場合でも，出力座標を論理座標へ戻した後に回転変換が適用されており，`resolve_coord` は拡大・回転の双方を考慮した正しい逆写像を実現できているといえる．

---

### 2.4 パターン走査機能の検証 (test_iterator.c)

#### 検証用プログラム

イテレータの初期化（`new_iter`），行優先走査（`pattern_iterator_next`），および水平優先走査（`pattern_iterator_next_h`）をまとめて検証するテストプログラムである．

```c
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
  int dummy[2][8][8] = {{{0}}};
  PatternView view = new_pattern_view(2, dummy);

  run_next(&view);
  putchar('\n');
  run_next_h(&view);

  return 0;
}
```

コンパイルおよび実行コマンド：

```sh
clang test_iterator.c fontlib.c -o test_iterator
./test_iterator
```

#### pattern_iterator_next (行優先走査) の実行結果と考察

##### 確認する機能

`PatternView` から初期化したイテレータが，行優先でセルイベント（`CELL`）・改行イベント（`NEWLINE`）・終了イベント（`END`）を正しく返す機能を確認する．

##### 実行結果（抜粋）

```text
pattern_iterator_next
CELL ch=0 src=(0,7)
CELL ch=0 src=(0,6)
CELL ch=0 src=(0,5)
CELL ch=0 src=(0,4)
CELL ch=0 src=(0,3)
CELL ch=0 src=(0,2)
CELL ch=0 src=(0,1)
CELL ch=0 src=(0,0)
NEWLINE
CELL ch=0 src=(1,7)
CELL ch=0 src=(1,6)
...
NEWLINE
CELL ch=1 src=(7,7)
CELL ch=1 src=(7,6)
CELL ch=1 src=(7,5)
CELL ch=1 src=(7,4)
CELL ch=1 src=(7,3)
CELL ch=1 src=(7,2)
CELL ch=1 src=(7,1)
CELL ch=1 src=(7,0)
END
```

##### 考察

行の終端（8セル目）に到達するごとに `NEWLINE` が発行され，1文字分（8行）の走査完了後に次の文字（`ch=1`）へ進み，全2文字の走査終了後に `END` が出力されている．途中で異常終了せず `END` に到達しているため，`new_iter` による初期化と反復終了条件の判定が正常に機能していることが確認できた．

#### pattern_iterator_next_h (水平優先走査) の実行結果と考察

##### 確認する機能

水平優先走査において，文字の切り替えと行の切り替えの順序が文字単位優先に切り替わることを確認する．

##### 実行結果（抜粋）

```text
pattern_iterator_next_h
CELL ch=0 src=(0,7)
CELL ch=0 src=(0,6)
CELL ch=0 src=(0,5)
CELL ch=0 src=(0,4)
CELL ch=0 src=(0,3)
CELL ch=0 src=(0,2)
CELL ch=0 src=(0,1)
CELL ch=0 src=(0,0)
CELL ch=1 src=(0,7)
CELL ch=1 src=(0,6)
CELL ch=1 src=(0,5)
CELL ch=1 src=(0,4)
CELL ch=1 src=(0,3)
CELL ch=1 src=(0,2)
CELL ch=1 src=(0,1)
CELL ch=1 src=(0,0)
NEWLINE
...
END
```

##### 考察

`pattern_iterator_next_h` の結果では，`ch=0` の第1行に続いてすぐに `ch=1` の第1行が走査され，その後に `NEWLINE` が発行されている．したがって，2つの反復関数は同じパターン列を対象としつつも，異なる順序で走査するという役割の違いを正しく実現できているといえる．

---

### 2.5 パターン描画制御機能の検証 (test_print_pattern.c)

#### 検証用プログラム

描画コンテキストの初期化（`new_pattern_view`, `new_pattern_view_doublescale`）と画面への文字パターン描画（`print_as_pattern`）を検証するテストプログラムである．

```c
#include <stdio.h>

#include "fontlib.h"

int main(void) {
  int patterns[1][8][8] = {{{0, 0, 1, 1, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0, 0, 0, 0},
                            {1, 1, 1, 1, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0}}};

  /* 等倍ビューの生成と検証 */
  PatternView view = new_pattern_view(1, patterns);
  printf("normal: h=%zu w=%zu scale_w=%u scale_h=%u rotation=%d\n", view.h,
         view.w, view.scale_w, view.scale_h, view.rotation);
  print_as_pattern(&view);

  /* 2倍スケールビューの生成と検証 */
  PatternView view2x = new_pattern_view_doublescale(1, patterns);
  printf("double: h=%zu w=%zu scale_w=%u scale_h=%u rotation=%d\n", view2x.h,
         view2x.w, view2x.scale_w, view2x.scale_h, view2x.rotation);
  print_as_pattern(&view2x);

  return 0;
}
```

コンパイルおよび実行コマンド：

```sh
clang test_print_pattern.c fontlib.c -o test_print_pattern
./test_print_pattern
```

#### new_pattern_view / new_pattern_view_doublescale の実行結果と考察

##### 確認する機能

標準倍率の `PatternView` と2倍スケールの `PatternView` が，仕様書で示された初期値を持つことを確認する．

##### 実行結果

```text
normal: h=8 w=8 scale_w=1 scale_h=1 rotation=1
double: h=8 w=8 scale_w=2 scale_h=2 rotation=1
```

※ `rotation=1` は `ROTATE_90` を表す．

##### 考察

`new_pattern_view` で生成したビューでは `h=8, w=8, scale_w=1, scale_h=1, rotation=1` が出力され，デフォルトサイズ・等倍スケール・90°回転という初期設定が正しく与えられている．また，`new_pattern_view_doublescale` では他の設定を引き継ぎつつ `scale_w=2, scale_h=2` のみに変更されており，拡大表示に必要な倍率設定だけを適切に変更できているといえる．

#### print_as_pattern の実行結果と考察

##### 確認する機能

`PatternView` に基づいてパターンを走査し，`view->string`（`"X"`）またはスペースを用いて大きな文字パターンを表示する機能を確認する．

##### 実行結果

```text
normal:
 XXXX
 XXXX
    XXXX
    XXXX
 XXXX
 XXXX


double:
  XXXXXXXX
  XXXXXXXX
  XXXXXXXX
  XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
  XXXXXXXX
  XXXXXXXX
  XXXXXXXX
  XXXXXXXX




```

##### 考察

通常表示時に入力した 8×8 パターンが `"X"` とスペースによって画面上に正しく描画されている．行ごとの並びを見ると，0 の部分は空白，1 の部分は `"X"` として出力されており，ビットパターンの形状が崩れずに再現されている．

さらに2倍表示時には，相似形を保ったまま各セルが縦横 2×2 文字に拡大されて出力されている．このことから，`print_as_pattern` は `PatternView` に含まれるスケール設定を正しく参照し，等倍および拡大表示の双方で適切な描画処理を行えていると確認できた．

---

## 3. 統合テストプログラム (main.c) の実行結果

### 3.1 パイプライン統合動作の検証

フォントデータファイルの読み込みから，パターン配列生成，拡大ビュー構築，画面への大きな文字パターン描画，およびメモリ解放までのパイプライン処理を一連で統合検証するプログラムである．

```c
#include <stdio.h>

#include "fontlib.h"

int main(void) {
  int (*patterns)[8][8];
  size_t n;

  int ret = load_txt_to_8x8("font_data02.txt", &patterns, &n);
  if (ret != 0) {
    fprintf(stderr, "failed to load font data: %d\n", ret);
    return 1;
  }

  PatternView view = new_pattern_view_doublescale(n, patterns);
  print_as_pattern(&view);

  free_memory(patterns);
}
```

コンパイルおよび実行コマンド：

```sh
clang main.c fontlib.c -o main
./main
```

---

### 3.2 実行結果と考察

#### 確認する機能

フォントデータファイル（`font_data02.txt`）を読み込み，2倍拡大表示ビューを用いて複数文字分のフォントパターンを連続描画し，正常に終了することを確認する．

#### 実行結果（抜粋）

```text
  XX
  XX
  XXXXXXXXXXXX
  XXXXXXXXXXXX
  XX    XX    XX
  XX    XX    XX
        XX    XX
        XX    XX
  XX    XX    XX
  XX    XX    XX
  XXXXXXXXXXXX
  XXXXXXXXXXXX
  XX
  XX


  XX          XX
  XX          XX
  XXXXXXXXXXXXXX
  XXXXXXXXXXXXXX
  XX    XX    XX
  XX    XX    XX
  XX    XX    XX
  XX    XX    XX
  XX    XX    XX
  XX    XX    XX
    XXXX  XXXX
    XXXX  XXXX
...
```

#### 考察

ファイル読み込みから回転・拡大を施した端末描画，メモリ解放までが連携して実行され，画面上にアルファベットの大文字パターンが縦横2倍サイズで正確に出力された．これにより，ライブラリ `fontlib` を構成する全機能が破綻なく協調して動作することが確認された．

---

## 4. 総合考察

本検証を通じて，以下の設計目標がすべて満たされていることが確認された．

1. **ライブラリ設計とテストプログラムの分離**: パターン表示に関するすべての基盤機能を `fontlib` に集約し，テストプログラム（機能別の `test_*.c` および統合検証用の `main.c`）をフラットに配置したことで，シンプルかつ堅牢なファイル構成を実現した．
2. **高い検証性と制約の遵守**: 各機能群に対応するテストプログラムを独立させ，ソースコード内でのマクロ定義（`#define`）やグローバル変数を一切使用しないクリーンな実装によって，高い可読性と再現性をもった検証が行えることを実証した．
3. **ビルドおよび運用の単純化**: サンプルコードをサブディレクトリに閉じ込めることなく同一階層に展開したことで，相対パス参照やインクルードパス指定が不要となり，初心者や外部環境でも極めて容易にビルド・検証できる構成となった．

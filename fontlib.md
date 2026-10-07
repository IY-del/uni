# パターン印刷ライブラリ (fontlib) 仕様書

学生番号：26G1012

氏名：石井陽泰

## 目次 <!-- omit in toc -->

- [1. ライブラリ概要](#1-ライブラリ概要)
   - [1.1. 概要](#11-概要)
   - [1.2. 機能構成と責務](#12-機能構成と責務)
   - [1.3. コンパイル方法](#13-コンパイル方法)
      - [直接コンパイル](#直接コンパイル)
      - [静的ライブラリの構築とリンク](#静的ライブラリの構築とリンク)
      - [動的ライブラリの構築とリンク](#動的ライブラリの構築とリンク)
- [2. 共通データ構造および列挙型定義](#2-共通データ構造および列挙型定義)
   - [2.1. 列挙型定義 (typedef enum)](#21-列挙型定義-typedef-enum)
      - [PatternRotation](#patternrotation)
      - [PatternEvent](#patternevent)
   - [2.2. 構造体定義 (typedef struct)](#22-構造体定義-typedef-struct)
      - [PatternCoord](#patterncoord)
      - [PatternSize](#patternsize)
      - [PatternView](#patternview)
      - [PatternStep](#patternstep)
      - [PatternIterator](#patterniterator)
- [3. 関数仕様](#3-関数仕様)
   - [3.1. 基本表示機能](#31-基本表示機能)
      - [printx](#printx)
      - [printxs](#printxs)
   - [3.2. パターン読み込み機能](#32-パターン読み込み機能)
      - [read\_bits (static)](#read_bits-static)
      - [pad\_to\_grid (static)](#pad_to_grid-static)
      - [load\_txt](#load_txt)
      - [load\_txt\_to\_8x8](#load_txt_to_8x8)
      - [free\_memory](#free_memory)
   - [3.3. 座標変換・サイズ計算機能](#33-座標変換サイズ計算機能)
      - [rotate\_coord (static)](#rotate_coord-static)
      - [pattern\_scaled\_size](#pattern_scaled_size)
      - [resolve\_coord](#resolve_coord)
   - [3.4. パターン走査機能](#34-パターン走査機能)
      - [new\_iter](#new_iter)
      - [pattern\_iterator\_next](#pattern_iterator_next)
      - [pattern\_iterator\_next\_h](#pattern_iterator_next_h)
   - [3.5. パターン描画制御機能](#35-パターン描画制御機能)
      - [new\_pattern\_view](#new_pattern_view)
      - [new\_pattern\_view\_doublescale](#new_pattern_view_doublescale)
      - [print\_as\_pattern](#print_as_pattern)
- [4. テストおよび実行結果](#4-テストおよび実行結果)

---

## 1. ライブラリ概要

### 1.1. 概要

本仕様書は，テキスト形式のフォントデータファイル（'0' および '1' のビット列）を読み込み，任意の拡大倍率（スケール）や回転方向（0°, 90°, 180°, 270°）を適用したうえで，端末画面（標準出力）上に大きな文字パターンとして描画するC言語ライブラリ **`fontlib`**（対象ファイル: `fontlib.c`，ヘッダー: `fontlib.h`）の仕様を規定する．

### 1.2. 機能構成と責務

`fontlib` は，責務ごとに以下の5つの機能群に分類されて実装されている．

| 機能群 | 役割・責務 | 主な提供関数 |
| --- | --- | --- |
| **基本表示機能** | 1セルの文字描画（`X` またはスペース，任意文字列）を行う基本出力機能 | `printx`, `printxs` |
| **パターン読み込み機能** | テキストファイルから0/1列を抽出し，動的配列としてパターンを確保・解放する機能 | `read_bits`, `pad_to_grid`, `load_txt`, `load_txt_to_8x8`, `free_memory` |
| **座標変換・サイズ計算機能** | 回転角度および拡大倍率に基づき，出力座標系から元パターン座標系への逆写像と出力寸法計算を行う機能 | `rotate_coord`, `pattern_scaled_size`, `resolve_coord` |
| **パターン走査機能** | 出力画面上の行優先または水平優先の走査状態を管理し，描画イベントを生成するイテレータ機能 | `new_iter`, `pattern_iterator_next`, `pattern_iterator_next_h` |
| **パターン描画制御機能** | パターン描画条件（サイズ，スケール，回転，文字種）を保持し，イテレータを用いて描画を実行する機能 | `new_pattern_view`, `new_pattern_view_doublescale`, `print_as_pattern` |

### 1.3. コンパイル方法

`fontlib` は，単一のヘッダーファイル `fontlib.h` と実装ファイル `fontlib.c` から構成される．利用側プログラムから利用する際は，以下の方式でコンパイルおよびリンクする．

#### 直接コンパイル

```sh
gcc -Wall -Wextra -O2 main.c fontlib.c -o app
```

#### 静的ライブラリの構築とリンク

```sh
# オブジェクトファイルの生成
gcc -Wall -Wextra -O2 -c fontlib.c

# 静的ライブラリ（libfont.a）の作成
ar rcs libfont.a fontlib.o

# アプリケーションとのリンク
gcc -Wall -Wextra -O2 main.c -L. -lfont -o app
```

#### 動的ライブラリの構築とリンク

```sh
# 位置独立コード（PIC）でオブジェクトファイルを生成
gcc -Wall -Wextra -O2 -fPIC -c fontlib.c

# 共有ライブラリ（libfont.so）の作成
gcc -shared -o libfont.so fontlib.o

# アプリケーションとのリンク
gcc -Wall -Wextra -O2 main.c -L. -lfont -o app

# 実行時（共有ライブラリ探索パスの指定）
LD_LIBRARY_PATH=. ./app
```

---

## 2. 共通データ構造および列挙型定義

### 2.1. 列挙型定義 (typedef enum)

動作モードや状態遷移を識別するための型として，以下の2つの列挙型を定義する．

#### PatternRotation

パターンの回転角度を指定する列挙型である．

```c
typedef enum {
  ROTATE_0,
  ROTATE_90,
  ROTATE_180,
  ROTATE_270
} PatternRotation;
```

| 識別子 | 値 | 説明 |
| --- | --- | --- |
| `ROTATE_0` | 0 | 回転なし（正立） |
| `ROTATE_90` | 1 | 時計回りに90度回転 |
| `ROTATE_180` | 2 | 180度回転（上下左右反転） |
| `ROTATE_270` | 3 | 時計回りに270度（反時計回りに90度）回転 |

#### PatternEvent

イテレータの走査状態および次に発生したイベント種別を表す列挙型である．

```c
typedef enum {
  PATTERN_EVENT_END,
  PATTERN_EVENT_CELL,
  PATTERN_EVENT_NEWLINE
} PatternEvent;
```

| 識別子 | 値 | 説明 |
| --- | --- | --- |
| `PATTERN_EVENT_END` | 0 | 全文字・全領域の走査完了（終了） |
| `PATTERN_EVENT_CELL` | 1 | 描画対象のセルへの到達（セル描画イベント） |
| `PATTERN_EVENT_NEWLINE` | 2 | 行末への到達（改行出力イベント） |

### 2.2. 構造体定義 (typedef struct)

#### PatternCoord

パターン上の2次元論理座標を表す構造体である．

```c
typedef struct {
  size_t x;
  size_t y;
} PatternCoord;
```

| メンバ名 | 型 | 意味 |
| --- | --- | --- |
| `x` | `size_t` | 元パターン上の水平インデックス（列番号: 0 〜 w-1） |
| `y` | `size_t` | 元パターン上の垂直インデックス（行番号: 0 〜 h-1） |

#### PatternSize

スケール倍率および回転を適用した後の出力矩形寸法を表す構造体である．

```c
typedef struct {
  size_t width;
  size_t height;
} PatternSize;
```

| メンバ名 | 型 | 意味 |
| --- | --- | --- |
| `width` | `size_t` | 画面出力時の全体の横幅（セル数） |
| `height` | `size_t` | 画面出力時の全体の縦幅（行数） |

#### PatternView

パターン表示に関するすべての描画パラメータ（データポインタ，寸法，スケール，回転，出力文字）を保持するコンテキスト構造体である．

```c
typedef struct {
  size_t count;
  size_t h;
  size_t w;
  void* pattern;
  const char* string;
  unsigned scale_w;
  unsigned scale_h;
  PatternRotation rotation;
} PatternView;
```

| メンバ名 | 型 | 意味 |
| --- | --- | --- |
| `count` | `size_t` | 格納されている文字パターン数 |
| `h` | `size_t` | 元パターン1文字あたりの高さ（行数） |
| `w` | `size_t` | 元パターン1文字あたりの幅（列数） |
| `pattern` | `void*` | パターンデータ配列への汎用ポインタ（通常は `int (*)[h][w]` 相当） |
| `string` | `const char*` | セル値が1のときに描画する文字列（標準: `\"X\"`） |
| `scale_w` | `unsigned` | 水平方向の拡大倍率（1以上の整数） |
| `scale_h` | `unsigned` | 垂直方向の拡大倍率（1以上の整数） |
| `rotation` | `PatternRotation` | 適用する回転角度 |

#### PatternStep

イテレータが走査中に返す1ステップの情報（イベント種別，対象文字番号，元パターン参照座標）を格納する構造体である．

```c
typedef struct {
  size_t ch;
  size_t src_x;
  size_t src_y;
  PatternEvent kind;
} PatternStep;
```

| メンバ名 | 型 | 意味 |
| --- | --- | --- |
| `ch` | `size_t` | 現在走査中の文字インデックス（0 〜 count-1） |
| `src_x` | `size_t` | 参照すべき元パターン上のX座標 |
| `src_y` | `size_t` | 参照すべき元パターン上のY座標 |
| `kind` | `PatternEvent` | 発生したイベント（CELL / NEWLINE / END） |

#### PatternIterator

走査処理の内部状態を管理するイテレータ構造体である．

```c
typedef struct {
  const PatternView* view;
  size_t ch;
  size_t out_w;
  size_t out_h;
  size_t x;
  size_t y;
} PatternIterator;
```

| メンバ名 | 型 | 意味 |
| --- | --- | --- |
| `view` | `const PatternView*` | 走査対象の描画設定ビューへのポインタ |
| `ch` | `size_t` | 現在処理中の文字インデックス |
| `out_w` | `size_t` | 1文字あたりの出力横幅（`pattern_scaled_size` より算出） |
| `out_h` | `size_t` | 1文字あたりの出力縦幅（`pattern_scaled_size` より算出） |
| `x` | `size_t` | 1文字内の現在走査中X位置（0 〜 out_w-1） |
| `y` | `size_t` | 1文字内の現在走査中Y位置（0 〜 out_h-1） |

---

## 3. 関数仕様

### 3.1. 基本表示機能

#### printx

- **説明**: パラメータ `flag` が0ならばスペース（`" "`）を1文字出力し，0以外の値ならば `"X"` を1文字標準出力に表示する．
- **パラメータ**:
   - `flag` (`int`): 表示内容を決定するフラグ（0で空白，0以外で `"X"`）
- **戻り値**: なし (`void`)
- **使用例**:

   ```c
  printx(1);
  printx(0);
  printx(2);
   ```

- **出力例**:

   ```text
  X X
   ```

#### printxs

- **説明**: パラメータ `flag` が0ならばスペース（`" "`）を1文字出力し，0以外の値ならば引数 `str` で与えられた文字列を表示する（`str == NULL` 時は安全に何もしない）．
- **パラメータ**:
   - `flag` (`int`): 表示内容を決定するフラグ（0で空白，0以外で `str`）
   - `str` (`const char str[]`): `flag` が0以外のときに表示対象となる文字列
- **戻り値**: なし (`void`)
- **使用例**:

   ```c
  printxs(1, "ABC");
  printxs(0, "ABC");
   ```

- **出力例**:

   ```text
  ABC 
   ```

---

### 3.2. パターン読み込み機能

#### read_bits (static)

- **説明**: ファイルを開き，ファイル内の `'0'` および `'1'` 文字のみを順次抽出して動的バッファに格納する内部関数．容量不足時は倍々拡張（`realloc`）を行い，読み込んだ配列ポインタとビット数を返却する．
- **パラメータ**:
   - `filename` (`const char*`): フォントデータファイルのパス
   - `out_buf` (`int**`): 確保された動的ビット配列の先頭ポインタの書き込み先
   - `out_len` (`size_t*`): 抽出されたビット数の書き込み先
- **戻り値**: `int`（正常終了時は `0`，ファイルオープン失敗時は `-1`，メモリ確保失敗時は `-2`）
- **使用例**:

   ```c
  int* bits = NULL;
  size_t len = 0;
  if (read_bits("font_data01.txt", &bits, &len) == 0) {
    printf("len = %zu\n", len);
    free(bits);
  }
   ```

- **出力例**:

   ```text
  len = 1663
   ```

#### pad_to_grid (static)

- **説明**: 読み込まれたビット列 `buf` と 1文字あたりのセル数 `cell`（$h \times w$）を受け取り，文字数 $n = (len + cell - 1) / cell$ を算出して `realloc` によりグリッド境界（$n \times cell$）へとインプレースリサイズし，末尾の不足分を `memset` で0クリア（パディング）する内部関数．
- **パラメータ**:
   - `buf` (`int*`): 読み込み済みビット配列の先頭ポインタ
   - `len` (`size_t`): 読み込まれたビット数
   - `cell` (`size_t`): 1文字あたりのセル総数（$h \times w$）
   - `out` (`int**`): 整形・パディング済み配列の先頭ポインタの書き込み先
   - `out_n` (`size_t*`): 計算された文字数 $n$ の書き込み先
- **戻り値**: `int`（正常終了時は `0`，メモリ確保失敗時は `-2`）
- **使用例**:

   ```c
  int* bits = calloc(10, sizeof(int));
  int* out = NULL;
  size_t n = 0;
  pad_to_grid(bits, 10, 8, &out, &n); /* cell=8 に対し n=2 (16要素) へパディング */
  printf("n = %zu\n", n);
  free(out);
   ```

- **出力例**:

   ```text
  n = 2
   ```

#### load_txt

- **説明**: 引数のバリデーションを行った後，内部関数 `read_bits` でファイルから生のビット列を抽出し，`pad_to_grid` で指定された $h \times w$ のセル境界に合わせたパディング配列を生成して呼び出し元へ返却する．
- **パラメータ**:
   - `filename` (`const char*`): フォントデータファイルのパス
   - `h` (`size_t`): 1文字あたりの高さ（行数，$h > 0$）
   - `w` (`size_t`): 1文字あたりの幅（列数，$w > 0$）
   - `out` (`int**`): 確保された1次元配列ポインタの書き込み先
   - `out_n` (`size_t*`): 読み込んだ文字数 $n$ の書き込み先
- **戻り値**: `int`（正常終了時は `0`，引数不正・ファイルオープン失敗時は `-1`，メモリ確保失敗時は `-2`）
- **使用例**:

   ```c
  int* arr = NULL;
  size_t n = 0;
  if (load_txt("font_data01.txt", 8, 8, &arr, &n) == 0) {
    printf("n = %zu\n", n);
    free_memory(arr);
  }
   ```

- **出力例**:

   ```text
  n = 26
   ```

#### load_txt_to_8x8

- **説明**: 内部で `load_txt(filename, 8, 8, &flat, out_n)` を呼び出し，確保された1次元配列を 8×8 固定の2次元配列ポインタ型 `int (**out)[8][8]` に安全にキャストして返却するラッパー関数．
- **パラメータ**:
   - `filename` (`const char filename[]`): フォントデータファイルのパス
   - `out` (`int (**)[8][8]`): 8×8 配列ポインタの書き込み先
   - `out_n` (`size_t*`): 読み込んだ文字数の書き込み先
- **戻り値**: `int`（正常終了時は `0`，失敗時は負数）
- **使用例**:

   ```c
  int (*patterns)[8][8] = NULL;
  size_t n = 0;
  if (load_txt_to_8x8("font_data01.txt", &patterns, &n) == 0) {
    printf("n = %zu, bit=%d\n", n, patterns[0][0][3]);
    free_memory(patterns);
  }
   ```

- **出力例**:

   ```text
  n = 26, bit=1
   ```

#### free_memory

- **説明**: `load_txt` または `load_txt_to_8x8` によって動的確保されたメモリ領域を解放する．`void*` を受け取るためキャスト不要で呼び出し可能であり，`NULL` が渡された場合も安全に何もしない．
- **パラメータ**:
   - `arr` (`void*`): 解放対象の動的メモリ領域アドレス
- **戻り値**: なし (`void`)
- **使用例**:

   ```c
  int* ptr = malloc(10 * sizeof(int));
  free_memory(ptr);
   ```

- **出力例**:

   ```text
  free_memory: success
   ```

---

### 3.3. 座標変換・サイズ計算機能

#### rotate_coord (static)

- **説明**: パターン上の論理座標 $(x, y)$ に対し，`view->rotation`（0°, 90°, 180°, 270°）に基づく回転写像を適用し，元パターン配列上の座標 $(src\_x, src\_y)$ を算出する内部関数．
- **パラメータ**:
   - `view` (`const PatternView*`): 描画設定ビューへのポインタ
   - `x` (`size_t`): 論理X座標
   - `y` (`size_t`): 論理Y座標
- **戻り値**: `PatternCoord`（元パターン座標）
- **使用例**:

   ```c
  int dummy[1][8][8] = {{{0}}};
  PatternView view = new_pattern_view(1, dummy);
  view.w = 8; view.h = 4;
  view.rotation = ROTATE_90;
  PatternCoord c = rotate_coord(&view, 2, 1);
   ```

- **出力例**:

   ```text
  rotate_coord(ROTATE_90, 2, 1): (1,1)
   ```

#### pattern_scaled_size

- **説明**: `view` の回転角度および拡大倍率を考慮し，1文字あたりの画面出力幅と高さを計算して `PatternSize` 構造体として返却する．`view == NULL` 時は `{0, 0}` を返す．
- **パラメータ**:
   - `view` (`const PatternView*`): 描画設定ビューへのポインタ
- **戻り値**: `PatternSize`（拡大・回転後の出力幅 `width` および高さ `height`）
- **使用例**:

   ```c
  int dummy[1][8][8] = {{{0}}};
  PatternView view = new_pattern_view(1, dummy);
  PatternSize s = pattern_scaled_size(&view);
  printf("w=%zu, h=%zu\n", s.width, s.height);
   ```

- **出力例**:

   ```text
  w=8, h=8
   ```

#### resolve_coord

- **説明**: 画面上の出力セル座標 $(out\_x, out\_y)$ を受け取り，拡大倍率で除算して論理座標を求めた後，`rotate_coord` により元パターン配列上の参照座標 $(src\_x, src\_y)$ を求めて返す．
- **パラメータ**:
   - `view` (`const PatternView*`): 描画設定ビューへのポインタ
   - `out_x` (`size_t`): 出力画面上の水平ピクセル座標
   - `out_y` (`size_t`): 出力画面上の垂直ピクセル座標
- **戻り値**: `PatternCoord`（元パターン配列上の参照座標）
- **使用例**:

   ```c
  int dummy[1][8][8] = {{{0}}};
  PatternView view = new_pattern_view(1, dummy);
  PatternCoord c = resolve_coord(&view, 0, 0);
  printf("resolve(0,0) -> (%zu,%zu)\n", c.x, c.y);
   ```

- **出力例**:

   ```text
  resolve(0,0) -> (0,7)
   ```

---

### 3.4. パターン走査機能

#### new_iter

- **説明**: `view` の設定に基づいて出力寸法を取得し，走査開始位置を原点（$x=0, y=0, ch=0$）に初期化した `PatternIterator` 構造体を生成して返却する．
- **パラメータ**:
   - `view` (`const PatternView*`): 走査対象の描画設定ビューへのポインタ
- **戻り値**: `PatternIterator`（初期化済みのイテレータ）
- **使用例**:

   ```c
  int dummy[1][8][8] = {{{0}}};
  PatternView view = new_pattern_view(1, dummy);
  PatternIterator it = new_iter(&view);
  printf("out_w=%zu, out_h=%zu\n", it.out_w, it.out_h);
   ```

- **出力例**:

   ```text
  out_w=8, out_h=8
   ```

#### pattern_iterator_next

- **説明**: 行優先走査（1文字ごとに全行を走査）を行い，次に描画すべきイベント（セル描画 `PATTERN_EVENT_CELL`，改行 `PATTERN_EVENT_NEWLINE`，走査終了 `PATTERN_EVENT_END`）を順次発行する．
- **パラメータ**:
   - `it` (`PatternIterator*`): イテレータ状態変数へのポインタ
   - `step` (`PatternStep*`): 次の走査結果情報を受け取る構造体へのポインタ
- **戻り値**: `PatternEvent`（発生したイベント種別）
- **使用例**:

   ```c
  int dummy[1][8][8] = {{{0}}};
  PatternView view = new_pattern_view(1, dummy);
  PatternIterator it = new_iter(&view);
  PatternStep step;
  if (pattern_iterator_next(&it, &step) == PATTERN_EVENT_CELL) {
    printf("ch=%zu, src=(%zu,%zu)\n", step.ch, step.src_x, step.src_y);
  }
   ```

- **出力例**:

   ```text
  ch=0, src=(0,7)
   ```

#### pattern_iterator_next_h

- **説明**: 水平優先走査（全文字の同一行を走査）を行い，1行分全文字のセルを出力した後に改行イベントを発行する方式で次の描画イベントを生成する．
- **パラメータ**:
   - `it` (`PatternIterator*`): イテレータ状態変数へのポインタ
   - `step` (`PatternStep*`): 次の走査結果情報を受け取る構造体へのポインタ
- **戻り値**: `PatternEvent`（発生したイベント種別）
- **使用例**:

   ```c
  int dummy[2][8][8] = {{{0}}};
  PatternView view = new_pattern_view(2, dummy);
  PatternIterator it = new_iter(&view);
  PatternStep step;
  if (pattern_iterator_next_h(&it, &step) == PATTERN_EVENT_CELL) {
    printf("h_ch=%zu, src=(%zu,%zu)\n", step.ch, step.src_x, step.src_y);
  }
   ```

- **出力例**:

   ```text
  h_ch=0, src=(0,7)
   ```

---

### 3.5. パターン描画制御機能

#### new_pattern_view

- **説明**: 文字数 `count` およびパターン配列ポインタ `pattern` をもとに，標準表示設定（等倍，回転90°，描画文字 `"X"`）を持つ `PatternView` 構造体を構築して返す．
- **パラメータ**:
   - `count` (`size_t`): 描画対象の文字パターン数
   - `pattern` (`void*`): パターン配列の先頭アドレス
- **戻り値**: `PatternView`（初期化済みの描画設定ビュー）
- **使用例**:

   ```c
  int dummy[1][8][8] = {{{0}}};
  PatternView view = new_pattern_view(1, dummy);
  printf("scale=%u, rot=%d\n", view.scale_w, view.rotation);
   ```

- **出力例**:

   ```text
  scale=1, rot=1
   ```

#### new_pattern_view_doublescale

- **説明**: `new_pattern_view` を呼び出した後，水平倍率 `scale_w` および垂直倍率 `scale_h` を 2倍に設定した `PatternView` 構造体を構築して返す．
- **パラメータ**:
   - `count` (`size_t`): 描画対象の文字パターン数
   - `pattern` (`void*`): パターン配列の先頭アドレス
- **戻り値**: `PatternView`（2倍スケール設定の描画ビュー）
- **使用例**:

   ```c
  int dummy[1][8][8] = {{{0}}};
  PatternView view2x = new_pattern_view_doublescale(1, dummy);
  printf("scale=%u\n", view2x.scale_w);
   ```

- **出力例**:

   ```text
  scale=2
   ```

#### print_as_pattern

- **説明**: `view` の設定に基づいてイテレータを駆動し，パターンデータを走査しながら標準出力に大きな文字パターンを描画する（描画セルには `view->string`，空白セルにはスペースを出力し，全文字走査後に改行を出力する）．
- **パラメータ**:
   - `view` (`const PatternView*`): 表示対象の描画設定ビューへのポインタ
- **戻り値**: なし (`void`)
- **使用例**:

   ```c
  int patterns[1][8][8] = {{{0, 0, 1, 1, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0, 0, 0, 0},
                            {1, 1, 1, 1, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {1, 1, 0, 0, 1, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0}}};
  PatternView view = new_pattern_view(1, patterns);
  print_as_pattern(&view);
   ```

- **出力例**:

   ```text
   XXXX   
   XXXX   
      XXXX
      XXXX
   XXXX   
   XXXX   
   ```

---

## 4. テストおよび実行結果

各機能のテスト項目，境界値検証，および統合プログラムの実行結果については，別冊の [パターン印刷プログラム 実行結果報告書](report.md) に集約している．

- **テストプログラムの検証結果**:
   - [基本表示機能のテスト (`test_display.c`)](report.md#21-基本表示機能の検証-test_displayc)
   - [パターン読み込み・メモリ管理機能のテスト (`test_load.c`)](report.md#22-パターン読み込みメモリ管理機能の検証-test_loadc)
   - [座標変換・サイズ計算機能のテスト (`test_coord.c`)](report.md#23-座標変換サイズ計算機能の検証-test_coordc)
   - [パターン走査機能のテスト (`test_iterator.c`)](report.md#24-パターン走査機能の検証-test_iteratorc)
   - [パターン描画制御機能のテスト (`test_view.c`)](report.md#25-パターン描画制御機能の検証-test_viewc)
- **統合実行プログラム結果**:
   - [実データによる統合描画プログラム (`main.c`)](report.md#3-統合実行プログラム-mainc-の検証結果)

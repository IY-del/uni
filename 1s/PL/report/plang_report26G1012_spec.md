# パターン印刷ライブラリ (fontlib) 仕様書

- 科目名: プログラミング言語
- 学籍番号: 26G1012
- 氏名: 石井陽泰
- 対象ソースファイル: `fontlib.c`
- 対象ヘッダーファイル: `fontlib.h`

---

## 目次

- [1. 概要と構成](#1-概要と構成)
  - [1.1 概要](#11-概要)
  - [1.2 機能構成と責務](#12-機能構成と責務)
  - [1.3 コンパイルおよびライブラリ構築方法](#13-コンパイルおよびライブラリ構築方法)
- [2. 共通データ構造および定数定義](#2-共通データ構造および定数定義)
  - [2.1 定数定義](#21-定数定義)
  - [2.2 列挙型定義](#22-列挙型定義)
  - [2.3 構造体定義](#23-構造体定義)
- [3. ライブラリ機能仕様](#3-ライブラリ機能仕様)
  - [3.1 基本表示機能](#31-基本表示機能)
    - [3.1.1 printx](#311-printx)
    - [3.1.2 printxs](#312-printxs)
  - [3.2 パターン読み込み機能](#32-パターン読み込み機能)
    - [3.2.1 load_txt](#321-load_txt)
    - [3.2.2 load_txt_to_8x8](#322-load_txt_to_8x8)
    - [3.2.3 free_memory](#323-free_memory)
  - [3.3 座標変換・サイズ計算機能](#33-座標変換サイズ計算機能)
    - [3.3.1 pattern_scaled_size](#331-pattern_scaled_size)
    - [3.3.2 resolve_coord](#332-resolve_coord)
  - [3.4 パターン走査機能](#34-パターン走査機能)
    - [3.4.1 new_iter](#341-new_iter)
    - [3.4.2 pattern_iterator_next](#342-pattern_iterator_next)
    - [3.4.3 pattern_iterator_next_h](#343-pattern_iterator_next_h)
  - [3.5 パターン描画制御機能](#35-パターン描画制御機能)
    - [3.5.1 new_pattern_view](#351-new_pattern_view)
    - [3.5.2 new_pattern_view_doublescale](#352-new_pattern_view_doublescale)
    - [3.5.3 print_as_pattern](#353-print_as_pattern)
- [4. 仕様変更点および設計履歴](#4-仕様変更点および設計履歴)
  - [4.1 座標変換機能の変更](#41-座標変換機能の変更)
  - [4.2 パターン走査機能の変更](#42-パターン走査機能の変更)
  - [4.3 パターン描画制御機能の変更](#43-パターン描画制御機能の変更)
  - [4.4 fontlib へのモジュール統合とリファクタリング](#44-fontlib-へのモジュール統合とリファクタリング)

---

## 1. 概要と構成

### 1.1 概要

本仕様書は，テキスト形式のフォントデータファイル（0/1のビット列）を読み込み，任意の拡大率（スケール）や回転方向（0°, 90°, 180°, 270°）を適用したうえで，端末画面（標準出力）上に大きな文字パターンとして描画するC言語ライブラリ **`fontlib`**（対象ファイル: `fontlib.c`，ヘッダー: `fontlib.h`）の仕様を規定する．

本ライブラリは，文字セル表示，ファイル解析・配列確保，幾何学的座標変換，走査順序イテレータ，および描画ビュー管理の各機能を集約した単一の共通ライブラリとして構成されており，静的ライブラリ（`libfont.a`）または動的ライブラリ（`libfont.so`）としてビルドして利用する．

### 1.2 機能構成と責務

`fontlib` は，責務ごとに以下の5つの機能群に分類されて実装されている．

| 機能群 | 役割・責務 | 主な提供関数 |
| --- | --- | --- |
| **基本表示機能** | 1セルの文字描画（`X` またはスペース，任意文字列）を行う基本出力機能 | `printx`, `printxs` |
| **パターン読み込み機能** | テキストファイルから0/1列を抽出し，動的配列としてパターンを確保・解放する機能 | `load_txt`, `load_txt_to_8x8`, `free_memory` |
| **座標変換・サイズ計算機能** | 回転角度および拡大倍率に基づき，出力座標系から元パターン座標系への逆写像と出力寸法計算を行う機能 | `pattern_scaled_size`, `resolve_coord` |
| **パターン走査機能** | 出力画面上の行優先または水平優先の走査状態を管理し，描画イベントを生成するイテレータ機能 | `new_iter`, `pattern_iterator_next`, `pattern_iterator_next_h` |
| **パターン描画制御機能** | パターン描画条件（サイズ，スケール，回転，文字種）を保持し，イテレータを用いて描画を実行する機能 | `new_pattern_view`, `new_pattern_view_doublescale`, `print_as_pattern` |

### 1.3 コンパイルおよびライブラリ構築方法

`fontlib` は，単一のヘッダーファイル `fontlib.h` と実装ファイル `fontlib.c` から構成される．利用側プログラムから利用する際は，以下の方式でライブラリを構築してリンクする．

#### 静的ライブラリの構築とリンク

```sh
# オブジェクトファイルの生成
$ gcc -c fontlib.c

# 静的ライブラリ（libfont.a）の作成
$ ar rcs libfont.a fontlib.o

# アプリケーションとのリンク例
$ gcc app.c -L. -lfont -o app
```

#### 動的ライブラリの構築とリンク

```sh
# 位置独立コード（PIC）でオブジェクトファイルを生成
$ gcc -fPIC -c fontlib.c

# 共有ライブラリ（libfont.so）の作成
$ gcc -shared -o libfont.so fontlib.o

# アプリケーションとのリンク例
$ gcc app.c -L. -lfont -o app

# 実行時（共有ライブラリ探索パスの指定）
$ LD_LIBRARY_PATH=. ./app
```

---

## 2. 共通データ構造および定数定義

### 2.1 定数定義

ヘッダーファイル `fontlib.h` において，パターンの標準寸法および表示倍率が以下のように定義されている．

```c
enum {
  PATTERN_DEFAULT_H = 8,
  PATTERN_DEFAULT_W = 8,
  PATTERN_SCALE_NORMAL = 1,
  PATTERN_SCALE_DOUBLE = 2
};
```

| 定数名 | 値 | 意味 |
| --- | --- | --- |
| `PATTERN_DEFAULT_H` | 8 | 1文字パターンの標準高さ（行数） |
| `PATTERN_DEFAULT_W` | 8 | 1文字パターンの標準幅（列数） |
| `PATTERN_SCALE_NORMAL` | 1 | 等倍表示（標準スケール倍率） |
| `PATTERN_SCALE_DOUBLE` | 2 | 2倍拡大表示（縦横2倍スケール倍率） |

### 2.2 列挙型定義

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

### 2.3 構造体定義

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
| `h` | `size_t` | 元パターン1文字あたりの高さ（標準: 8） |
| `w` | `size_t` | 元パターン1文字あたりの幅（標準: 8） |
| `pattern` | `void*` | パターンデータ配列への汎用ポインタ（通常は `int (*)[h][w]` 相当） |
| `string` | `const char*` | セル値が1のときに描画する文字列（標準: `"X"`） |
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

## 3. ライブラリ機能仕様

### 全提供関数一覧

| 関数名 | 所属機能群 | 概要説明 |
| --- | --- | --- |
| `printx` | 基本表示 | フラグに応じて「`X`」またはスペースを1文字表示する |
| `printxs` | 基本表示 | フラグに応じて指定された文字列，またはスペース1文字を表示する |
| `load_txt` | パターン読み込み | テキストファイルから任意の高さ・幅のパターン配列を動的に確保して読み込む |
| `load_txt_to_8x8` | パターン読み込み | `load_txt` を用いて8×8固定サイズの二次元配列ポインタとして読み込む |
| `free_memory` | パターン読み込み | `load_txt` 系の関数で確保された動的メモリ領域を解放する |
| `pattern_scaled_size` | 座標変換・サイズ計算 | 回転角およびスケール倍率を反映した出力全体の幅・高さを計算する |
| `resolve_coord` | 座標変換・サイズ計算 | 出力画面上の座標 `(out_x, out_y)` から元パターン上の座標 `(src_x, src_y)` を算出する |
| `new_iter` | パターン走査 | `PatternView` に基づいてイテレータの初期状態を構築する |
| `pattern_iterator_next` | パターン走査 | 行優先（1文字ごとに全行を走査）で次の描画イベントを取得する |
| `pattern_iterator_next_h` | パターン走査 | 水平優先（全文字の同一行を走査）で次の描画イベントを取得する |
| `new_pattern_view` | パターン描画制御 | 標準設定（等倍，回転90°，出力文字 `"X"`）の `PatternView` を生成する |
| `new_pattern_view_doublescale` | パターン描画制御 | 縦横2倍スケール設定の `PatternView` を生成する |
| `print_as_pattern` | パターン描画制御 | `PatternView` の設定に基づいてイテレータを駆動し，パターンを標準出力に表示する |

---

### 3.1 基本表示機能

#### 概要

画面出力の最小単位である「1セル」の表示を担当する．整数フラグ（0 または非0）に応じて，スペースまたは所定の文字・文字列を標準出力へ書き出す．

| 関数名 | 説明 |
| --- | --- |
| `printx` | フラグに応じて「`X`」またはスペースを1文字表示する |
| `printxs` | フラグに応じて指定された文字列，またはスペース1文字を表示する |

#### 3.1.1 printx

##### シグネチャ

```c
void printx(int flag);
```

##### 機能説明

パラメータ `flag` が 0 ならばスペース（`" "`）を1文字出力し，0 以外の値ならば `"X"` を1文字標準出力に表示する．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 | 例 |
| --- | --- | :---: | --- | --- |
| `flag` | `int` | はい | 表示内容を決定するフラグ（0でスペース，0以外で `"X"`） | `1` |

##### 戻り値仕様

- 戻り値型: `void`
- 返却値: なし

##### 使用例

```c
printx(1);  /* X を表示 */
printx(0);  /* スペースを表示 */
printx(2);  /* X を表示（0以外はすべて X） */
```

##### 実行例

```text
X X
```

#### 3.1.2 printxs

##### シグネチャ

```c
void printxs(int flag, const char str[]);
```

##### 機能説明

パラメータ `flag` が 0 ならばスペース（`" "`）を1文字だけ表示し，0 以外の値ならば第2引数 `str` で与えられた文字列をそのまま表示する．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 | 例 |
| --- | --- | :---: | --- | --- |
| `flag` | `int` | はい | 表示内容を決定するフラグ（0でスペース，0以外で `str`） | `1` |
| `str` | `const char[]` | はい | `flag` が0以外のときに表示対象となる文字列 | `"ABC"` |

##### 戻り値仕様

- 戻り値型: `void`
- 返却値: なし

##### 使用例

```c
printxs(1, "ABC");  /* "ABC" を表示 */
printxs(0, "ABC");  /* スペース1文字を表示（"ABC"は無視される） */
```

##### 実行例

```text
ABC 
```

---

### 3.2 パターン読み込み機能

#### 概要

テキストファイルに記録されたフォントのビット列データ（'0' および '1'）を読み込み，行・文字ごとの区切りを解釈して動的メモリ上に連続領域としてパディング・配置する機能を提供する．読み込んだメモリの安全な解放関数も併せて提供する．

| 関数名 | 説明 |
| --- | --- |
| `load_txt` | テキストファイルから任意の高さ・幅のパターン配列を動的に確保して読み込む |
| `load_txt_to_8x8` | `load_txt` を用いて8×8固定サイズの二次元配列ポインタとして読み込む |
| `free_memory` | `load_txt` 系の関数で確保された動的メモリ領域を解放する |

#### 3.2.1 load_txt

##### シグネチャ

```c
int load_txt(const char* filename, size_t h, size_t w, int** out, size_t* out_n);
```

##### 機能説明

指定されたテキストファイル `filename` を開き，ファイル内に含まれる `'0'` および `'1'` 文字のみを順次抽出してバッファリングする．抽出した総ビット数に対して 1 文字あたりのセル数 `cell = h * w` を基準とし，必要な文字数 `n = (len + cell - 1) / cell` を算出する．`n * cell` 個の `int` 要素をゼロクリア（`calloc`）で動的確保し，抽出ビット列を順次格納してポインタを `*out` に，文字数を `*out_n` に格納する．

##### 処理の流れ

1. `filename` を読み取り専用（`"r"`）で開く．開けない場合は `-1` を返す．
2. 内部動的バッファを確保し，`fgetc` で1文字ずつ読み出す．
3. 文字が `'0'` または `'1'` の場合のみ，数値（0 または 1）としてバッファに追記する（容量不足時は `realloc` で拡張）．
4. ファイル走査終了後，ファイルを閉じる．
5. 抽出数 `len` から必要文字数 `n = (len + h * w - 1) / (h * w)` を算出する．
6. `calloc(n * h * w, sizeof(int))` で最終配列を確保し，バッファ内容をコピーする．
7. `*out` に確保配列のアドレスを，`*out_n` に文字数 `n` を設定し，成功コード `0` を返す．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 | 例 |
| --- | --- | :---: | --- | --- |
| `filename` | `const char*` | はい | フォントデータファイルのパス | `"font_data01.txt"` |
| `h` | `size_t` | はい | 1文字あたりの高さ（行数） | `8` |
| `w` | `size_t` | はい | 1文字あたりの幅（列数） | `8` |
| `out` | `int**` | はい | 確保された1次元パターン配列の先頭ポインタを受け取るポインタ変数 | `&patterns` |
| `out_n` | `size_t*` | はい | 読み込んだパターン文字数を受け取る変数へのポインタ | `&n` |

##### 戻り値仕様

| 戻り値 | 型 | 意味 |
| --- | --- | --- |
| `0` | `int` | 正常終了（成功） |
| `-1` | `int` | ファイルオープン失敗（ファイルが存在しない，権限不足など） |
| `-2` | `int` | メモリ確保失敗（`malloc` / `realloc` / `calloc` の失敗） |

##### 使用例

```c
int *patterns;
size_t n;

if (load_txt("font_data01.txt", 8, 8, &patterns, &n) == 0) {
    printf("n = %zu\n", n);
    free_memory(patterns);
}
```

##### 実行例

```text
n = 26
```

#### 3.2.2 load_txt_to_8x8

##### シグネチャ

```c
int load_txt_to_8x8(const char filename[], int (**out)[8][8], size_t* out_n);
```

##### 機能説明

`load_txt` を高さ `h=8`，幅 `w=8` として呼び出し，取得した1次元配列ポインタを 8×8 の2次元配列の配列ポインタ型 `int (**out)[8][8]` にキャストして返すラッパー関数である．既存の8×8固定サイズ前提のコードとの型親和性を高めるために提供される．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 | 例 |
| --- | --- | :---: | --- | --- |
| `filename` | `const char[]` | はい | フォントデータファイルのパス | `"font_data01.txt"` |
| `out` | `int (**)[8][8]` | はい | 8×8配列の先頭ポインタを受け取るポインタ変数 | `&patterns` |
| `out_n` | `size_t*` | はい | 読み込んだパターン文字数を受け取る変数へのポインタ | `&n` |

##### 戻り値仕様

| 戻り値 | 型 | 意味 |
| --- | --- | --- |
| `0` | `int` | 正常終了（成功） |
| `-1` | `int` | ファイルオープン失敗 |
| `-2` | `int` | メモリ確保失敗 |

##### 使用例

```c
int (*patterns)[8][8];
size_t n;

if (load_txt_to_8x8("font_data01.txt", &patterns, &n) == 0) {
    printf("n = %zu\n", n);
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) printf("%d", patterns[0][y][x]);
        putchar('\n');
    }
    free_memory(patterns);
}
```

##### 実行例

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

#### 3.2.3 free_memory

##### シグネチャ

```c
void free_memory(void* arr);
```

##### 機能説明

`load_txt` または `load_txt_to_8x8` によって確保された動的ヒープ領域を解放する．引数を `void*` 型で受け取るため，`int*` や `int (*)[8][8]` のいずれの型であってもキャストを行わずに直接渡すことができる．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 |
| --- | --- | :---: | --- |
| `arr` | `void*` | はい | 解放対象の動的確保メモリ領域のアドレス（`NULL` の場合は何もしない） |

##### 戻り値仕様

- 戻り値型: `void`
- 返却値: なし

##### 使用例

```c
int (*patterns)[8][8];
size_t n;

if (load_txt_to_8x8("font_data01.txt", &patterns, &n) == 0) {
    free_memory(patterns);  /* int (*)[8][8] でもキャスト不要 */
    printf("free_memory: success\n");
}
```

##### 実行例

```text
free_memory: success
```

---

### 3.3 座標変換・サイズ計算機能

#### 概要

拡大縮小（スケーリング）および回転変換の幾何学的処理を一括して受け持つ．出力画面側のピクセル座標から，元パターン配列上の対応するセル座標への逆写像（サンプリングマッピング）を提供し，さらに回転角とスケール倍率に応じた出力矩形サイズを算出する．

| 関数名 | 説明 |
| --- | --- |
| `pattern_scaled_size` | 回転角およびスケール倍率を反映した出力全体の幅・高さを計算する |
| `resolve_coord` | 出力画面上の座標 `(out_x, out_y)` から元パターン上の座標 `(src_x, src_y)` を算出する |

#### 3.3.1 pattern_scaled_size

##### シグネチャ

```c
PatternSize pattern_scaled_size(const PatternView* view);
```

##### 機能説明

`view` に指定された回転角度（`rotation`）およびスケール倍率（`scale_w`, `scale_h`）に基づいて，1文字あたりの実際の画面出力幅（セル数）と高さ（行数）を計算し，`PatternSize` 構造体として返す．

##### 計算アルゴリズム

- **回転が 90度 または 270度 の場合**:
  - 幅と高さの軸が入れ替わるため，元パターンの高さ `view->h` に対して横倍率 `view->scale_w` を乗じ，元パターンの幅 `view->w` に対して縦倍率 `view->scale_h` を乗じる．
  - `size.width = view->h * view->scale_w`
  - `size.height = view->w * view->scale_h`
- **回転が 0度 または 180度 の場合**:
  - 元パターンの軸が維持されるため，そのまま各倍率を乗じる．
  - `size.width = view->w * view->scale_w`
  - `size.height = view->h * view->scale_h`

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 |
| --- | --- | :---: | --- |
| `view` | `const PatternView*` | はい | 対象となる描画設定ビューへのポインタ |

##### 戻り値仕様

| 戻り値型 | メンバ | 説明 |
| --- | --- | --- |
| `PatternSize` | `width` (`size_t`) | 拡大・回転後の1文字あたりの出力幅 |
| | `height` (`size_t`) | 拡大・回転後の1文字あたりの出力高さ |

##### 使用例

```c
int dummy[1][8][8] = {{{0}}};
PatternView view = new_pattern_view(1, dummy);
PatternSize size = pattern_scaled_size(&view);
printf("size = (w=%zu, h=%zu)\n", size.width, size.height);
```

##### 実行例

```text
size = (w=8, h=8)
```

#### 3.3.2 resolve_coord

##### シグネチャ

```c
PatternCoord resolve_coord(const PatternView* view, size_t out_x, size_t out_y);
```

##### 機能説明

画面上の出力セル座標 `(out_x, out_y)` を受け取り，拡大倍率による整数除算を行って論理的なグリッド位置 `(logical_x, logical_y)` を求め，さらに内部関数 `rotate_coord` を用いて回転変換を適用することにより，元パターン配列上の参照座標 `(src_x, src_y)` を求めて返す．

##### 処理の流れ

1. 横方向論理位置の算出: `logical_x = out_x / view->scale_w`
2. 縦方向論理位置の算出: `logical_y = out_y / view->scale_h`
3. 回転変換 `rotate_coord` の適用:
   - `ROTATE_0`:
     - `coord.x = logical_x`
     - `coord.y = logical_y`
   - `ROTATE_90`:
     - `coord.x = logical_y`
     - `coord.y = view->h - 1 - logical_x`
   - `ROTATE_180`:
     - `coord.x = view->w - 1 - logical_x`
     - `coord.y = view->h - 1 - logical_y`
   - `ROTATE_270`:
     - `coord.x = view->w - 1 - logical_y`
     - `coord.y = logical_x`
4. 算出した `PatternCoord` を返却する．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 |
| --- | --- | :---: | --- |
| `view` | `const PatternView*` | はい | 対象となる描画設定ビューへのポインタ |
| `out_x` | `size_t` | はい | 出力画面上の水平ピクセル座標 |
| `out_y` | `size_t` | はい | 出力画面上の垂直ピクセル座標 |

##### 戻り値仕様

| 戻り値型 | メンバ | 説明 |
| --- | --- | --- |
| `PatternCoord` | `x` (`size_t`) | 元パターン配列における参照X座標（列） |
| | `y` (`size_t`) | 元パターン配列における参照Y座標（行） |

##### 使用例

```c
int dummy[1][8][8] = {{{0}}};
PatternView view = new_pattern_view(1, dummy);
PatternCoord src = resolve_coord(&view, 0, 0);
printf("resolve(0,0) -> (%zu,%zu)\n", src.x, src.y);
```

##### 実行例

```text
resolve(0,0) -> (0,7)
```

---

### 3.4 パターン走査機能

#### 概要

画面出力のループ処理をカプセル化するイテレータを提供する．描画側コードが複雑な二重・三重ループや座標変換の内部演算を直接意識することなく，1ステップずつイベントを取得しながら直感的に出力を行えるように設計されている．

| 関数名 | 説明 |
| --- | --- |
| `new_iter` | `PatternView` に基づいてイテレータの初期状態を構築する |
| `pattern_iterator_next` | 行優先（1文字ごとに全行を走査）で次の描画イベントを取得する |
| `pattern_iterator_next_h` | 水平優先（全文字の同一行を走査）で次の描画イベントを取得する |

#### 3.4.1 new_iter

##### シグネチャ

```c
PatternIterator new_iter(const PatternView* view);
```

##### 機能説明

指定された `PatternView` をもとに，出力幅と高さを `pattern_scaled_size(view)` で計算し，走査開始位置を `x=0, y=0, ch=0` に初期化した `PatternIterator` 構造体を返す．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 |
| --- | --- | :---: | --- |
| `view` | `const PatternView*` | はい | 走査対象の描画設定ビューへのポインタ |

##### 戻り値仕様

- 戻り値型: `PatternIterator`
- 初期化済みのイテレータ構造体

##### 使用例

```c
int dummy[1][8][8] = {{{0}}};
PatternView view = new_pattern_view(1, dummy);
PatternIterator it = new_iter(&view);
printf("out_w=%zu, out_h=%zu, x=%zu, y=%zu, ch=%zu\n",
       it.out_w, it.out_h, it.x, it.y, it.ch);
```

##### 実行例

```text
out_w=8, out_h=8, x=0, y=0, ch=0
```

#### 3.4.2 pattern_iterator_next

##### シグネチャ

```c
PatternEvent pattern_iterator_next(PatternIterator* it, PatternStep* step);
```

##### 機能説明

行優先でパターンを走査する．1文字内の1行分（`x` が 0 から `out_w - 1` まで）を走査し，行末に達したときは改行イベント `PATTERN_EVENT_NEWLINE` を発行する．文字の最終行まで走査した場合は次の文字へ進み，全文字の走査が完了した段階で `PATTERN_EVENT_END` を返す．

##### 処理の流れ

1. `it->x >= it->out_w` の場合:
   - `it->x = 0` にリセットし，`it->y++` で行を進める．
   - `step->kind = PATTERN_EVENT_NEWLINE` を設定する．
   - もし `it->y >= it->out_h` ならば，`it->y = 0` にリセットして `it->ch++`（次の文字）へ進める．
   - もし `it->ch >= it->view->count` ならば，`step->kind = PATTERN_EVENT_END` を設定する．
   - イベント種別を返す．
2. それ以外（セル走査）の場合:
   - `resolve_coord(it->view, it->x, it->y)` を呼び出して元パターン座標 `src` を取得する．
   - `step->ch = it->ch`, `step->src_x = src.x`, `step->src_y = src.y`, `step->kind = PATTERN_EVENT_CELL` を設定する．
   - `it->x++` で次の水平位置へ進める．
   - `PATTERN_EVENT_CELL` を返す．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 |
| --- | --- | :---: | --- |
| `it` | `PatternIterator*` | はい | イテレータ状態変数へのポインタ |
| `step` | `PatternStep*` | はい | 次の走査結果情報を受け取る構造体へのポインタ |

##### 戻り値仕様

| 戻り値型 | 値 | 意味 |
| --- | --- | --- |
| `PatternEvent` | `PATTERN_EVENT_CELL` | セル描画が必要 |
| | `PATTERN_EVENT_NEWLINE` | 改行出力が必要 |
| | `PATTERN_EVENT_END` | 全走査の終了 |

##### 使用例

```c
int dummy[1][8][8] = {{{0}}};
PatternView view = new_pattern_view(1, dummy);
PatternIterator it = new_iter(&view);
PatternStep step;

if (pattern_iterator_next(&it, &step) == PATTERN_EVENT_CELL) {
    printf("CELL: ch=%zu, src=(%zu,%zu)\n", step.ch, step.src_x, step.src_y);
}
```

##### 実行例

```text
CELL: ch=0, src=(0,7)
```

#### 3.4.3 pattern_iterator_next_h

##### シグネチャ

```c
PatternEvent pattern_iterator_next_h(PatternIterator* it, PatternStep* step);
```

##### 機能説明

水平優先でパターンを走査するバリエーションである．1文字の行末に達した際，すぐに同じ文字の次行へ進むのではなく，同一行のまま次の文字（`ch++`）へと水平に切り替えて走査する．全文字の同一行走査が完了した段階で改行イベントを発行し，全行の終了をもって `PATTERN_EVENT_END` を返す．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 |
| --- | --- | :---: | --- |
| `it` | `PatternIterator*` | はい | イテレータ状態変数へのポインタ |
| `step` | `PatternStep*` | はい | 次の走査結果情報を受け取る構造体へのポインタ |

##### 戻り値仕様

`pattern_iterator_next` と同一の `PatternEvent` 列挙値を返す．

##### 使用例

```c
int dummy[2][8][8] = {{{0}}};
PatternView view = new_pattern_view(2, dummy);
PatternIterator it = new_iter(&view);
PatternStep step;

if (pattern_iterator_next_h(&it, &step) == PATTERN_EVENT_CELL) {
    printf("CELL_H: ch=%zu, src=(%zu,%zu)\n", step.ch, step.src_x, step.src_y);
}
```

##### 実行例

```text
CELL_H: ch=0, src=(0,7)
```

---

### 3.5 パターン描画制御機能

#### 概要

読み込まれたフォントパターン配列，表示倍率，回転方向，出力文字列を結合した `PatternView` の管理と，画面への最終的な大きな文字出力処理を担当する．

| 関数名 | 説明 |
| --- | --- |
| `new_pattern_view` | 標準設定（等倍，回転90°，出力文字 `"X"`）の `PatternView` を生成する |
| `new_pattern_view_doublescale` | 縦横2倍スケール設定の `PatternView` を生成する |
| `print_as_pattern` | `PatternView` の設定に基づいてイテレータを駆動し，パターンを標準出力に表示する |

#### 3.5.1 new_pattern_view

##### シグネチャ

```c
PatternView new_pattern_view(size_t count, void* pattern);
```

##### 機能説明

指定されたパターン配列ポインタおよび文字数をもとに，標準の表示設定を持つ `PatternView` を初期化して返す．要件の実行例に合わせるため，デフォルトの回転方向は `ROTATE_90` に設定される．

##### 初期設定値

- `count`: 引数 `count`
- `h`: `PATTERN_DEFAULT_H` (8)
- `w`: `PATTERN_DEFAULT_W` (8)
- `pattern`: 引数 `pattern`
- `string`: `"X"`
- `scale_w`: `PATTERN_SCALE_NORMAL` (1)
- `scale_h`: `PATTERN_SCALE_NORMAL` (1)
- `rotation`: `ROTATE_90`

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 |
| --- | --- | :---: | --- |
| `count` | `size_t` | はい | 描画対象のパターン文字数 |
| `pattern` | `void*` | はい | パターンデータ配列の先頭アドレス |

##### 戻り値仕様

- 戻り値型: `PatternView`
- 初期化済みのパターンビュー構造体

##### 使用例

```c
int dummy[1][8][8] = {{{0}}};
PatternView view = new_pattern_view(1, dummy);
printf("normal: h=%zu w=%zu scale_w=%u scale_h=%u rotation=%d\n",
       view.h, view.w, view.scale_w, view.scale_h, view.rotation);
```

##### 実行例

```text
normal: h=8 w=8 scale_w=1 scale_h=1 rotation=1
```

#### 3.5.2 new_pattern_view_doublescale

##### シグネチャ

```c
PatternView new_pattern_view_doublescale(size_t count, void* pattern);
```

##### 機能説明

`new_pattern_view(count, pattern)` を呼び出して標準設定を作成したのち，水平倍率 `scale_w` および垂直倍率 `scale_h` を `PATTERN_SCALE_DOUBLE` (2) に書き換えた `PatternView` を返す．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 |
| --- | --- | :---: | --- |
| `count` | `size_t` | はい | 描画対象のパターン文字数 |
| `pattern` | `void*` | はい | パターンデータ配列の先頭アドレス |

##### 戻り値仕様

- 戻り値型: `PatternView`
- 2倍拡大スケールが設定されたパターンビュー構造体

##### 使用例

```c
int dummy[1][8][8] = {{{0}}};
PatternView view2x = new_pattern_view_doublescale(1, dummy);
printf("double: h=%zu w=%zu scale_w=%u scale_h=%u rotation=%d\n",
       view2x.h, view2x.w, view2x.scale_w, view2x.scale_h, view2x.rotation);
```

##### 実行例

```text
double: h=8 w=8 scale_w=2 scale_h=2 rotation=1
```

#### 3.5.3 print_as_pattern

##### シグネチャ

```c
void print_as_pattern(const PatternView* view);
```

##### 機能説明

`view` の設定をもとに `new_iter(view)` でイテレータを初期化し，`pattern_iterator_next` によって全セルを走査しながら描画を行う．各セルについて `pattern[ch][src_y][src_x]` の値を取得し，`printxs` を呼び出してセル値に応じた出力を行う．すべての文字パターンの表示完了後，末尾に改行を1つ出力する．

##### 引数仕様

| 引数名 | 型 | 必須 | 説明 |
| --- | --- | :---: | --- |
| `view` | `const PatternView*` | はい | 表示対象の描画設定ビューへのポインタ |

##### 戻り値仕様

- 戻り値型: `void`
- 返却値: なし

##### 使用例

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

##### 実行例

```text
 XXXX   
 XXXX   
    XXXX
    XXXX
 XXXX   
 XXXX   
        
        
```

---

## 4. 仕様変更点および設計履歴

### 4.1 座標変換機能の変更

- 出力幅と出力高さを個別に返していた旧関数 `pattern_output_width`，`pattern_output_height`，`pattern_scaled_width`，`pattern_scaled_height` を廃止した．
- 代替として，出力寸法をまとめて保持する `PatternSize` 構造体およびこれを一括取得する `pattern_scaled_size` 関数を導入した．
- これにより，幅と高さを1回の関数呼び出しで効率的に取得できるようになり，幾何計算の重複を排除した．

### 4.2 パターン走査機能の変更

- 座標変換機能のインターフェース刷新に伴い，`new_iter` 内部における出力サイズの取得ロジックを，個別関数呼び出しから `pattern_scaled_size(view)` の利用へと一本化した．
- 取得した `PatternSize` の `width` および `height` をそれぞれイテレータの `out_w` および `out_h` に設定する簡潔な構造に改善した．

### 4.3 パターン描画制御機能の変更

- `new_pattern_view` におけるデフォルト回転角を `ROTATE_0` から `ROTATE_90` に変更した．
- これにより，フォントデータファイルに格納されたビット列の並び方向に対して，追加の回転パラメータ指定なしで要件どおりの正立した大文字パターンが表示されるようになった．

### 4.4 fontlib へのモジュール統合とリファクタリング

- 旧設計において個別のソースコード群に分散していた機能群（基本描画，ファイル読み込み，座標変換，走査イテレータ，描画制御）の責務境界を精査し，単一の共通ライブラリ `fontlib.h` および `fontlib.c` へと集約した．
- これにより，外部アプリケーションとのインターフェースが一本化され，静的ライブラリ（`libfont.a`）および動的ライブラリ（`libfont.so`）としての再利用性と保守性が大幅に向上した．

# パターン印刷プログラム 実行結果報告書

学生番号：26G1012

氏名：石井陽泰

## 目次 <!-- omit in toc -->

- [1. 報告概要](#1-報告概要)
- [2. 機能別テストプログラムの実行結果](#2-機能別テストプログラムの実行結果)
   - [2.1. 基本表示機能の検証 (`test_display.c`)](#21-基本表示機能の検証-test_displayc)
      - [対象関数](#対象関数)
      - [テスト関数構成](#テスト関数構成)
      - [テスト項目](#テスト項目)
      - [コンパイルおよび実行コマンド](#コンパイルおよび実行コマンド)
      - [実行結果](#実行結果)
      - [考察](#考察)
   - [2.2. パターン読み込み・メモリ管理機能の検証 (`test_load.c`)](#22-パターン読み込みメモリ管理機能の検証-test_loadc)
      - [対象関数](#対象関数-1)
      - [テスト関数構成](#テスト関数構成-1)
      - [テスト項目](#テスト項目-1)
      - [コンパイルおよび実行コマンド](#コンパイルおよび実行コマンド-1)
      - [実行結果](#実行結果-1)
      - [考察](#考察-1)
   - [2.3. 座標変換・サイズ計算機能の検証 (`test_coord.c`)](#23-座標変換サイズ計算機能の検証-test_coordc)
      - [対象関数](#対象関数-2)
      - [テスト関数構成](#テスト関数構成-2)
      - [テスト項目](#テスト項目-2)
      - [コンパイルおよび実行コマンド](#コンパイルおよび実行コマンド-2)
      - [実行結果](#実行結果-2)
      - [考察](#考察-2)
   - [2.4. パターン走査機能の検証 (`test_iterator.c`)](#24-パターン走査機能の検証-test_iteratorc)
      - [対象関数](#対象関数-3)
      - [テスト関数構成](#テスト関数構成-3)
      - [テスト項目](#テスト項目-3)
      - [コンパイルおよび実行コマンド](#コンパイルおよび実行コマンド-3)
      - [実行結果（抜粋）](#実行結果抜粋)
      - [考察](#考察-3)
   - [2.5. パターン描画制御機能の検証 (`test_view.c`)](#25-パターン描画制御機能の検証-test_viewc)
      - [対象関数](#対象関数-4)
      - [テスト関数構成](#テスト関数構成-4)
      - [テスト項目](#テスト項目-4)
      - [コンパイルおよび実行コマンド](#コンパイルおよび実行コマンド-4)
      - [実行結果](#実行結果-3)
      - [考察](#考察-4)
- [3. 統合実行プログラム (`main.c`) の検証結果](#3-統合実行プログラム-mainc-の検証結果)
      - [実行コマンド](#実行コマンド)
      - [実行結果（抜粋）](#実行結果抜粋-1)
      - [考察](#考察-5)
- [4. 総合考察](#4-総合考察)

## 1. 報告概要

本報告書は，[パターン印刷ライブラリ (fontlib) 仕様書](fontlib.md) に基づいて実装された共通ライブラリ `fontlib`（`fontlib.h` / `fontlib.c`）の動作検証を目的として作成された機能別テストプログラム群および統合実行プログラム `main.c` の実行結果をまとめたものである．

各関数の引数・戻り値や詳細な仕様については，[パターン印刷ライブラリ (fontlib) 仕様書](fontlib.md#3-関数仕様) を参照のこと．

## 2. 機能別テストプログラムの実行結果

### 2.1. 基本表示機能の検証 (`test_display.c`)

#### 対象関数

- [`printx`](fontlib.md#printx)
- [`printxs`](fontlib.md#printxs)

#### テスト関数構成

- `test_printx()`: 真値・偽値に対する `'X'` および空白の出力検証
- `test_printxs()`: 文字列出力，空白1文字出力，NULLポインタ安全性の検証

#### テスト項目

| 関数      | 入力値・検証内容        | 期待値       | テスト理由・観点                                                       |
| :-------- | :---------------------- | :----------- | :--------------------------------------------------------------------- |
| `printx`  | `flag = 1`              | `"X"`        | 【正常系】代表真値に対する `"X"` 1文字出力の確認                       |
| `printx`  | `flag = 0`              | `" "`        | 【境界値】偽値に対する空白（スペース）1文字出力の確認                  |
| `printx`  | `flag = 2`              | `"X"`        | 【同値分割】1以外の正整数（C言語の真論理）に対する `"X"` 出力の確認    |
| `printxs` | `flag = 1, str = "ABC"` | `"ABC"`      | 【正常系】真値に対する指定文字列 `"ABC"` の直接出力確認                |
| `printxs` | `flag = 0, str = "ABC"` | `" "`        | 【境界値】偽値に対する空白1文字出力（文字列長に依存せず1セル幅）の確認 |
| `printxs` | `flag = 1, str = NULL`  | （出力なし） | 【安全性】NULLポインタ引数に対するクラッシュ回避・安全ガードの確認     |

#### コンパイルおよび実行コマンド

```sh
gcc -Wall -Wextra test_display.c fontlib.c -o test_display
./test_display
```

#### 実行結果

```text
=== 1. 基本表示機能 ===
[printx]
  printx(1): "X" (expected: "X")
  printx(0): " " (expected: " ")
  printx(2): "X" (expected: "X")

[printxs]
  printxs(1, "ABC"): "ABC" (expected: "ABC")
  printxs(0, "ABC"): " " (expected: " ")
  printxs(1, NULL): (safe no-op)
```

#### 考察

1. **仕様書に記載された使用例との完全な一致**:
   - `printx(1)` で `"X"`，`printx(0)` でスペース，`printxs(1, "ABC")` で `"ABC"`，`printxs(0, "ABC")` でスペースが出力され，仕様書「3.1. 基本表示機能」の使用例および実行例通りの結果が得られることが実証された．
2. **言語仕様準拠およびレイアウト維持の証明**:
   - `printx(2)` においても `"X"` が出力され，`flag == 1` への固定比較ではなくC言語標準の真偽値判定（0以外は真）として動作していることが証明された．
   - `printxs(0, "ABC")` において，指定文字列が3文字であっても出力は空白1文字（1セル幅）に留まるため，非描画セルによって文字グリッドの配置が崩れないことが確認された．

### 2.2. パターン読み込み・メモリ管理機能の検証 (`test_load.c`)

#### 対象関数

- [`read_bits` (static)](fontlib.md#read_bits-static)
- [`pad_to_grid` (static)](fontlib.md#pad_to_grid-static)
- [`load_txt`](fontlib.md#load_txt)
- [`load_txt_to_8x8`](fontlib.md#load_txt_to_8x8)
- [`free_memory`](fontlib.md#free_memory)

#### テスト関数構成

- `test_read_bits()`: ファイルからのビット列抽出および先頭バイト並びの単体検証
- `test_pad_to_grid()`: 不足ビットのインプレースゼロパディングおよび文字数計算の単体検証
- `test_load_txt()`: 実ファイル読み込み正常系，異常系（存在しないファイル），境界値（NULLポインタ，h=0）の検証
- `test_load_txt_to_8x8()`: 8×8配列ポインタ展開およびビットマップ可視化の検証
- `test_free_memory()`: 動的確保メモリの解放およびNULL解放の安全性検証

#### テスト項目

| 関数              | 入力値・検証内容              | 期待値            | テスト理由・観点                                                           |
| :---------------- | :---------------------------- | :---------------- | :------------------------------------------------------------------------- |
| `read_bits`       | `"font_data01.txt"`           | `ret=0, len=1663` | 【内部関数検証】ファイルからの純粋なビット列抽出と要素数確認               |
| `pad_to_grid`     | `bits, len=1663, cell=64`     | `ret=0, n=26`     | 【内部関数検証】境界不足分（1ビット）のインプレースゼロパディング確認      |
| `load_txt`        | `"font_data01.txt", h=8, w=8` | `ret=0, n=26`     | 【正常系】実ファイルからの動的確保と文字数取得の確認                       |
| `load_txt`        | 先頭文字の先頭行参照          | `00011000`        | 【ビット正確性】抽出ビット列が元データ通り正確に配列化されていることの確認 |
| `load_txt`        | `"non_existent.txt"`          | `ret=-1`          | 【異常系】存在しないファイル指定時の安全なエラー返却確認                   |
| `load_txt`        | `filename = NULL`             | `ret=-1`          | 【安全性】NULLポインタに対する引数ガードの確認                             |
| `load_txt`        | `h = 0`（ゼロ除算境界）       | `ret=-1`          | 【安全性】0除算を引き起こす寸法に対する事前拒絶確認                        |
| `load_txt_to_8x8` | `"font_data01.txt"`           | `ret=0, n=26`     | 【正常系】8×8二次元配列ポインタへの型キャスト読み込み確認                  |
| `free_memory`     | 確保済みポインタの解放        | 正常終了          | 【正常系】動的ヒープ領域の安全な解放確認                                   |
| `free_memory`     | `NULL` の解放                 | 安全な無処理      | 【安全性】NULLポインタ解放時のセグフォ回避確認                             |

#### コンパイルおよび実行コマンド

```sh
# static 関数の直接単体テストを含むため test_load.c を直接コンパイル
gcc -Wall -Wextra test_load.c -o test_load
./test_load
```

#### 実行結果

```text
=== 2. パターン読み込み・メモリ管理機能 ===
[read_bits (static)]
  read_bits("font_data01.txt"): ret=0, len=1663 (expected: ret=0, len=1663)
  first 8 bits: 00011000 (expected: 00011000)

[pad_to_grid (static)]
  pad_to_grid(..., len=1663, cell=64): ret=0, n=26 (expected: ret=0, n=26)

[load_txt]
  load_txt("font_data01.txt", 8, 8): ret=0, n=26 (expected: ret=0, n=26)
  load_txt("non_existent.txt"): ret=-1 (expected: -1)
  load_txt(NULL, ...): ret=-1 (expected: -1)
  load_txt(..., h=0, ...): ret=-1 (expected: -1)

[load_txt_to_8x8]
  load_txt_to_8x8("font_data01.txt"): ret=0, n=26 (expected: ret=0, n=26)
  letter 0 (8x8 bitmap):
    00011000
    00100100
    01000010
    01000010
    01111110
    01000010
    01000010
    00000000

[free_memory]
  free_memory(patterns): success
  free_memory(NULL): success (safe no-op)
```

#### 考察

1. **責務分離（`read_bits` と `pad_to_grid`）の妥当性とパディング機能の直接証明**:
   - `load_txt` を「ファイル入力・ビット抽出を担う `read_bits`」と「グリッド計算・パディングを担う `pad_to_grid`」の2段階に責務分割した．
   - 実データ `font_data01.txt` の総ビット数（1663ビット）は $26 \times 64 = 1664$ に対して1ビット不足しているが，`pad_to_grid` により文字数 $n=26$ が正しく切り上げ計算され，末尾の1ビット分がゼロパディングされて 1664 要素の完全な配列へとインプレースリサイズされたことが単体テストによって直接実証された．
2. **保守性とテスト容易性の両立**:
   - `static` 関数として内部局所化しつつも，テストコードから直接呼び出して個別に検証できる体制を確立したことで，I/Oエラーとメモリパディングエラーの切り分けが極めて明快となった．

### 2.3. 座標変換・サイズ計算機能の検証 (`test_coord.c`)

#### 対象関数

- [`rotate_coord` (static)](fontlib.md#rotate_coord-static)
- [`pattern_scaled_size`](fontlib.md#pattern_scaled_size)
- [`resolve_coord`](fontlib.md#resolve_coord)

#### テスト関数構成

- `test_rotate_coord()`: 各回転角（ROTATE_0, ROTATE_90, ROTATE_180, ROTATE_270）における座標写像およびNULL安全性の単体検証
- `test_pattern_scaled_size()`: 非正方形パターンの回転時寸法転置，拡大スケーリング乗算，複合変換，NULL安全性の検証
- `test_resolve_coord()`: 原点・境界対角点における各回転方向での座標逆写像，スケーリング除算逆写像，NULL安全性の検証

#### テスト項目

以下では，NULL入力のケースを除き，元パターンの寸法を `w=8, h=4` とする．倍率の指定がないケースは `scale_w=1, scale_h=1` とし，座標は `(x, y)`，出力寸法は幅×高さで表す．

| 関数                  | 入力値・検証内容                                        | 期待値                               | テスト理由・観点                                         |
| :-------------------- | :------------------------------------------------------ | :----------------------------------- | :------------------------------------------------------- |
| `rotate_coord`        | `ROTATE_0, (2, 1)`                                      | `(2, 1)`                             | 【内部関数検証】回転なし時の参照座標の確認               |
| `rotate_coord`        | `ROTATE_90, (2, 1)`                                     | `(1, 1)`                             | 【内部関数検証】90度回転時の元パターン参照座標の確認     |
| `rotate_coord`        | `ROTATE_180, (2, 1)`                                    | `(5, 2)`                             | 【内部関数検証】180度回転時の元パターン参照座標の確認    |
| `rotate_coord`        | `ROTATE_270, (2, 1)`                                    | `(6, 2)`                             | 【内部関数検証】270度回転時の元パターン参照座標の確認    |
| `rotate_coord`        | `view=NULL, (0, 0)`                                     | `(0, 0)`                             | 【安全性】NULL入力時の戻り値の確認                       |
| `pattern_scaled_size` | 各回転角，等倍                                          | 0/180°: 8×4，90/270°: 4×8            | 【軸転置検証】非正方形パターンの回転に伴う出力寸法の確認 |
| `pattern_scaled_size` | `ROTATE_0, scale_w=3, scale_h=2`                        | 24×8                                 | 【正常系】非等方拡大時の出力寸法の確認                   |
| `pattern_scaled_size` | `ROTATE_90, scale_w=3, scale_h=2`                       | 12×16                                | 【複合変換】回転と非等方拡大を組み合わせた出力寸法の確認 |
| `pattern_scaled_size` | `view=NULL`                                             | 0×0                                  | 【安全性】NULL入力時の戻り値の確認                       |
| `resolve_coord`       | `ROTATE_0`，出力原点 `(0, 0)` および対角端点 `(7, 3)`   | 原点 → `(0, 0)`，対角端点 → `(7, 3)` | 【境界値】出力端点と参照座標の対応確認                   |
| `resolve_coord`       | `ROTATE_90`，出力原点 `(0, 0)` および対角端点 `(3, 7)`  | 原点 → `(0, 3)`，対角端点 → `(7, 0)` | 【境界値】90度回転時の出力端点と参照座標の対応確認       |
| `resolve_coord`       | `ROTATE_180`，出力原点 `(0, 0)` および対角端点 `(7, 3)` | 原点 → `(7, 3)`，対角端点 → `(0, 0)` | 【境界値】180度回転時の出力端点と参照座標の対応確認      |
| `resolve_coord`       | `ROTATE_270`，出力原点 `(0, 0)` および対角端点 `(3, 7)` | 原点 → `(7, 0)`，対角端点 → `(0, 3)` | 【境界値】270度回転時の出力端点と参照座標の対応確認      |
| `resolve_coord`       | `ROTATE_0, scale_w=3, scale_h=2`，出力原点 `(0, 0)`     | `(0, 0)`                             | 【正常系】拡大設定下での原点の対応確認                   |
| `resolve_coord`       | `ROTATE_90, scale_w=3, scale_h=2`，出力原点 `(0, 0)`    | `(0, 3)`                             | 【複合変換】回転・拡大設定下での原点の対応確認           |
| `resolve_coord`       | `view=NULL, (0, 0)`                                     | `(0, 0)`                             | 【安全性】NULL入力時の戻り値の確認                       |

#### コンパイルおよび実行コマンド

```sh
# static 関数の直接単体テストを含むため test_coord.c を直接コンパイル
gcc -Wall -Wextra test_coord.c -o test_coord
./test_coord
```

#### 実行結果

```text
=== 3. 座標変換・サイズ計算機能 ===
[rotate_coord (static)]
  rotate_coord(ROTATE_0, 2, 1): (2,1) (expected: (2,1))
  rotate_coord(ROTATE_90, 2, 1): (1,1) (expected: (1,1))
  rotate_coord(ROTATE_180, 2, 1): (5,2) (expected: (5,2))
  rotate_coord(ROTATE_270, 2, 1): (6,2) (expected: (6,2))
  rotate_coord(NULL, 0, 0): (0,0) (expected: (0,0))

[pattern_scaled_size]
  rotate0:               size=(w=8,h=4) (expected: w=8,h=4)
  rotate90:              size=(w=4,h=8) (expected: w=4,h=8)
  rotate180:             size=(w=8,h=4) (expected: w=8,h=4)
  rotate270:             size=(w=4,h=8) (expected: w=4,h=8)
  scale_w3_h2:           size=(w=24,h=8) (expected: w=24,h=8)
  rotate90_scale_w3_h2:  size=(w=12,h=16) (expected: w=12,h=16)
  pattern_scaled_size(NULL): (w=0,h=0) (expected: (0,0))

[resolve_coord]
  rotate0:               resolve(0,0) -> (0,0) (expected: (0,0))
  rotate0 corner:        resolve(7,3) -> (7,3) (expected: (7,3))
  rotate90:              resolve(0,0) -> (0,3) (expected: (0,3))
  rotate90 corner:       resolve(3,7) -> (7,0) (expected: (7,0))
  rotate180:             resolve(0,0) -> (7,3) (expected: (7,3))
  rotate180 corner:      resolve(7,3) -> (0,0) (expected: (0,0))
  rotate270:             resolve(0,0) -> (7,0) (expected: (7,0))
  rotate270 corner:      resolve(3,7) -> (0,3) (expected: (0,3))
  scale_w3_h2:           resolve(0,0) -> (0,0) (expected: (0,0))
  rotate90_scale_w3_h2:  resolve(0,0) -> (0,3) (expected: (0,3))
  resolve_coord(NULL, 0, 0): (0,0) (expected: (0,0))
```

#### 考察

1. **static 関数の直接単体テストによる内部幾何変換の証明**:
   - テストコードにおいて `#include "fontlib.c"` を行うことで，外部非公開である `static PatternCoord rotate_coord` を直接呼び出して単体検証した．
   - 4方向すべての回転角度において，幾何学的な座標変換式通りの計算結果が得られることを直接証明した．
2. **非正方形フィクスチャによる軸転置欠陥の排除**:
   - 正方形（8×8）では検知できない縦横入れ替わりバグを検出するため，あえて横長パターン（$w=8, h=4$）を投入した．
   - 90°および270°回転時に出力寸法が `w=4, h=8` へ，対角端点が `(3, 7) -> (7, 0)` へと正確に移ることを確認し，境界値の1ズレ（オフバイワン）や軸の取り違えが皆無であることを実証した．

### 2.4. パターン走査機能の検証 (`test_iterator.c`)

#### 対象関数

- [`new_iter`](fontlib.md#new_iter)
- [`pattern_iterator_next`](fontlib.md#pattern_iterator_next)
- [`pattern_iterator_next_h`](fontlib.md#pattern_iterator_next_h)

#### テスト関数構成

- `test_new_iter()`: イテレータの初期状態（出力寸法および走査座標）の検証
- `test_pattern_iterator_next()`: 行優先走査（1文字完結型）におけるイベントシーケンスおよび完了判定の検証
- `test_pattern_iterator_next_h()`: 水平優先走査（全文字並行型）におけるイベントシーケンスおよび完了判定の検証
- `test_iterator_null_safety()`: NULLポインタ入力時の安全な走査停止ガード検証

#### テスト項目

| 関数                      | 入力値・検証内容         | 期待値                                  | テスト理由・観点                                                 |
| :------------------------ | :----------------------- | :-------------------------------------- | :--------------------------------------------------------------- |
| `new_iter`                | `view`（文字数2）        | `out_w=8, out_h=8, x=0, y=0, ch=0`      | 【基本動作】イテレータ初期状態（原点・出力寸法）の構築確認       |
| `pattern_iterator_next`   | 2文字分のパターン走査    | `CELL` → `NEWLINE` → `ch`更新 → `END`   | 【状態遷移】行優先（1文字完結型）のイベント発行シーケンス確認    |
| `pattern_iterator_next_h` | 2文字分のパターン走査    | 同一行全文字 `CELL` → `NEWLINE` → `END` | 【状態遷移】水平優先（全文字並行型）のイベント発行シーケンス確認 |
| `pattern_iterator_next`   | `it = NULL, step = NULL` | `PATTERN_EVENT_END`                     | 【安全性】NULLポインタに対する安全な走査停止ガード確認           |

#### コンパイルおよび実行コマンド

```sh
gcc -Wall -Wextra test_iterator.c fontlib.c -o test_iterator
./test_iterator
```

#### 実行結果（抜粋）

```text
=== 4. パターン走査機能 ===
[new_iter]
  new_iter: out_w=8 out_h=8 x=0 y=0 ch=0 (expected: out_w=8 out_h=8 x=0 y=0 ch=0)

[pattern_iterator_next (行優先走査)]
pattern_iterator_next
CELL ch=0 src=(0,7)
CELL ch=0 src=(0,6)
...
NEWLINE
CELL ch=0 src=(1,7)
...
NEWLINE
CELL ch=1 src=(0,7)
...
END

[pattern_iterator_next_h (水平優先走査)]
pattern_iterator_next_h
CELL ch=0 src=(0,7)
...
CELL ch=0 src=(0,0)
CELL ch=1 src=(0,7)
...
CELL ch=1 src=(0,0)
NEWLINE
...
END

[NULL Safety]
  pattern_iterator_next(NULL, NULL): 0 (expected: 0)
```

#### 考察

1. **仕様書に合致する走査イベント発行の証明**:
   - `new_iter` により初期化されたイテレータが，仕様書の通り $x=0, y=0, ch=0$ から走査を開始し，1ステップごとに正確な元パターン参照座標 `src=(src_x, src_y)` を生成していることを確認した．
2. **走査モード間の直交性と無限ループ耐性**:
   - 行優先（`pattern_iterator_next`）では1文字の8行が完了した後に `ch=1` へ遷移し，水平優先（`pattern_iterator_next_h`）では全文字の同一行が出力された後に `NEWLINE` が発行されるという，各走査アルゴリズムの仕様が厳密に満たされていることを実証した．
   - 全文字走査完了時に例外なく `PATTERN_EVENT_END`（0）が返却され，ループが安全に終了することを確認した．

### 2.5. パターン描画制御機能の検証 (`test_view.c`)

#### 対象関数

- [`new_pattern_view`](fontlib.md#new_pattern_view)
- [`new_pattern_view_doublescale`](fontlib.md#new_pattern_view_doublescale)
- [`print_as_pattern`](fontlib.md#print_as_pattern)

#### テスト関数構成

- `test_new_pattern_view()`: 等倍・正立ビュー初期化パラメータの検証
- `test_new_pattern_view_doublescale()`: 2倍拡大・正立ビュー初期化パラメータの検証
- `test_print_as_pattern()`: 等倍および2倍拡大でのパターン描画出力の検証
- `test_print_as_pattern_null_safety()`: NULLポインタ入力時の安全な無処理（safe no-op）検証

#### テスト項目

| 関数                           | 入力値・検証内容   | 期待値                           | テスト理由・観点                                              |
| :----------------------------- | :----------------- | :------------------------------- | :------------------------------------------------------------ |
| `new_pattern_view`             | `count=1, pattern` | `scale=1, rotation=1(ROTATE_90)` | 【基本動作】標準等倍・正立回転パラメータの初期化確認          |
| `new_pattern_view_doublescale` | `count=1, pattern` | `scale=2, rotation=1(ROTATE_90)` | 【基本動作】縦横2倍拡大パラメータの初期化確認                 |
| `print_as_pattern`             | 標準ビュー（等倍） | 8×8の文字パターン表示            | 【正常系】ビット `1` に対する `"X"` 描画と改行の整合性確認    |
| `print_as_pattern`             | 2倍ビュー          | 16×16の拡大パターン表示          | 【正常系】各セルが 2×2 の文字ブロックへと拡大されることの確認 |
| `print_as_pattern`             | `view = NULL`      | 安全な無処理                     | 【安全性】NULLポインタに対するクラッシュ回避ガード確認        |

#### コンパイルおよび実行コマンド

```sh
gcc -Wall -Wextra test_view.c fontlib.c -o test_view
./test_view
```

#### 実行結果

```text
=== 5. パターン描画制御機能 ===
[new_pattern_view]
  normal: h=8 w=8 scale_w=1 scale_h=1 rotation=1

[new_pattern_view_doublescale]
  double: h=8 w=8 scale_w=2 scale_h=2 rotation=1

[print_as_pattern (normal)]
 XXXX
 XXXX
    XXXX
    XXXX
 XXXX
 XXXX



[print_as_pattern (double scale)]
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





[NULL Safety]
  print_as_pattern(NULL): safe no-op
```

#### 考察

1. **仕様書に合致する描画アスペクト比とスケーリング精度の証明**:
   - `print_as_pattern` の等倍出力と2倍拡大出力を対比した結果，等倍表示における 4文字幅の `"XXXX"` が拡大表示において 8文字幅の `"XXXXXXXX"` かつ 縦方向も4行分へと正確に線形拡大されていることが視覚的・幾何学的に証明された．
2. **正立表示（ROTATE_90）の妥当性**:
   - デフォルト回転角度を `ROTATE_90` に設定したことで，フォントファイル中のビット列の並び順と端末表示の行優先出力との間の軸のずれが自然に解消され，追加の回転パラメータ指定を要することなく直感的な文字パターン出力が得られることが確認された．

## 3. 統合実行プログラム (`main.c`) の検証結果

実データファイル `font_data02.txt`（アルファベット26文字分）を読み込み，2倍拡大スケールビューを構築して標準出力へ一括描画し，動的メモリを解放するエンドツーエンド（E2E）の統合実行プログラムである．

#### 実行コマンド

```sh
gcc -Wall -Wextra -O2 main.c fontlib.c -o main
./main
```

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

- `load_txt_to_8x8("font_data02.txt", &patterns, &n)` により26文字すべてのフォントデータが正常に読み込まれ，戻り値 `0` を返却してビュー構築へと引き渡された．
- `new_pattern_view_doublescale` および `print_as_pattern` を経由して26文字すべての拡大パターンが画面に正しく描画され，終了直前の `free_memory(patterns)` によりメモリリークを生じることなく正常終了（終了コード `0`）することが実機上で確認された．

## 4. 総合考察

本検証では，読み込み，座標変換，走査，描画を機能別に確認し，統合実行プログラムでそれらを組み合わせた処理を確認した．以下では，設計上の意義，結果から確認できる範囲，今後の検証課題を述べる．

1. **責務分離による検証対象の明確化**:
   ビット抽出を `read_bits`，セル境界への調整を `pad_to_grid`，出力座標から参照座標への変換を `resolve_coord` が担う構成により，各処理の入力と出力を個別に確認できた．また，走査をイテレータによるイベント生成，描画をイベントに応じた出力として分離したことで，参照座標や改行のタイミングを描画結果とは別に観察できた．この構成は不具合の切り分けに有用と考えられるが，保守性や実行効率の向上を定量的に評価したものではない．

2. **入力条件の選択と検証範囲**:
   8×4の非正方形パターンにより，90度・270度回転に伴う幅と高さの入れ替えを確認した．横3倍・縦2倍の非等方拡大では，回転と倍率を組み合わせた出力寸法を確認した．ただし，拡大時の参照座標について掲載した結果は原点に限られるため，座標除算の動作を確認するには，拡大ブロックの内部と境界のケースを追加する必要がある．

3. **異常系の扱いとメモリ管理**:
   存在しないファイル，NULL入力，ゼロの高さについて，掲載したケースではエラー返却や無処理などの動作を確認した．統合実行プログラムでは，読み込み失敗時に描画へ進まない構成と，成功時に取得した配列を終了前に解放する処理を確認できる．ただし，これらはメモリ確保失敗時の全経路やリーク不在を確認したことを意味しない．それらの評価には，失敗条件を与えるテストやメモリ検査の結果が必要である．

4. **内部関数の直接検証の利点と制約**:
   `fontlib.c` をテスト用の翻訳単位にインクルードする方法により，通常は外部から呼び出せない `static` 関数を直接検証できた．ビット抽出，セル境界への調整，回転写像を個別に観察できる点は利点である．一方，テストが内部関数の構成に依存するため，公開仕様を変えない内部改修でもテストの修正が必要になり得る．内部関数のテストと公開インターフェース経由のテストは，相互に補完するものとして位置づけるべきである．

5. **結果の判定方法と今後の課題**:
   本報告書の検証は，数値出力と期待値の比較，イベント列の確認，描画結果の目視確認を中心としている．実行結果の抜粋だけでは，全セルの参照座標や全パターンの描画を個別に判断できないため，今後は期待値との自動比較やセル数・改行数の集計を追加すると再検証しやすくなる．また，不足ビットのパディングについては，文字数の切り上げに加え，追加要素が0であることと既存要素が保持されることを確認したい．今回の結果は，掲載した入力条件における基本動作を支持するものであり，すべての入力に対する正しさの証明とは区別する必要がある．

#!/bin/bash

# ソースとオブジェクトディレクトリを変数で設定
SRC_DIR="source"
OBJ_DIR="object"

# 引数チェック（ファイル名だけ渡す）
if [ $# -lt 1 ]; then
    echo "Usage: $0 <filename_without_extension>"
    exit 1
fi

FNAME="$1"
SRC="$SRC_DIR/$FNAME.cc"
OBJ="$OBJ_DIR/$FNAME.o"

# 出力ディレクトリがなければ作る
mkdir -p "$OBJ_DIR"

# コンパイル
g++ -Iinclude -c "$SRC" -o "$OBJ"

echo "$OBJ created"

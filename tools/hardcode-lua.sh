#!/bin/bash
dir=$(dirname $0)
luac=${dir}/luajit-compile
hardcode=${dir}/hardcode.sh
pathname=$1
tmp=$(mktemp)
$luac < $pathname > $tmp
filename=$(basename $pathname)
$hardcode $tmp ${dir}/../src/niu2x/hardcode/lua_${filename/.*}


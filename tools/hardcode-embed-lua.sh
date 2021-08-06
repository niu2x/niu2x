#!/bin/bash
set -e
dir=$(dirname $0)
luac=${dir}/luajit-compile
hardcode=${dir}/hardcode.sh

find ${dir}/../src/embed_lua -name "*.lua"|while read pathname; do
	tmp=$(mktemp)
	echo "process ${pathname}"
	$luac < $pathname > $tmp
	filename=$(basename $pathname)
	$hardcode $tmp ${dir}/../src/niu2x/hardcode/lua_${filename/.*}
	rm ${tmp}
done


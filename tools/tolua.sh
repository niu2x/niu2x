#!/bin/bash
set -e

if [ "$#" -ne 1 ]; then
    echo "too few arguments"
fi

basic=$(mktemp)

cat << EOF > ${basic}

_basic['std::string'] = 'nxcppstring'
_basic['const std::string'] = 'nxcppstring'
_basic_ctype['nxcppstring'] = 'std::string'

_basic['nx_luafunction'] = 'nx_luafunction'
_basic_ctype['nx_luafunction'] = 'nx_luafunction'

_basic['uint16_t'] = 'number'
_basic['const uint16_t'] = 'number'
_basic['size_t'] = 'number'
_basic['const size_t'] = 'number'

EOF

workdir=$1
find $workdir -name '*.pkg'|while read item; do
	filename=$(basename $item);
	dir=$(dirname $item);
	pkgname=${filename/.*}
	tolua++5.1 -n $pkgname -H ${dir}/tolua_${pkgname}.h  -S -L ${basic} $item > ${dir}/tolua_${pkgname}.cpp
	sed 's/};/}/g' -i ${dir}/tolua_${pkgname}.cpp
	# sed 's/tolua_pushcppstring(tolua_S,(const char\*)/my_pushcppstring(tolua_S, /g' -i ${dir}/tolua_${pkgname}.cpp
	# sed '13i#define my_pushcppstring(L, sz) lua_pushlstring(L,sz.c_str(), sz.length())' -i ${dir}/tolua_${pkgname}.cpp
	# sed '1i#pragma GCC diagnostic ignored "-Werror=unused-function"' -i ${dir}/tolua_${pkgname}.cpp
	clang-format -i ${dir}/tolua_${pkgname}.cpp
	clang-format -i ${dir}/tolua_${pkgname}.h
done

rm ${basic}
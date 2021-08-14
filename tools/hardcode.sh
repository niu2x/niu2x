#!/bin/bash
set -e
resource=$1
output=$2
namespace=$3
echo "hardcode ${resource} to ${output}"

dir=$(dirname ${output})
file=$(basename ${output})

bin2cpp ${resource} ${output} cpp

sed -i '3i#include <niu2x/utils.h>' ${output}.h
sed -i "9inamespace ${namespace} {" ${output}.h
sed -i '12i}' ${output}.h
sed -i "12iconst memref ${file}_mref={${file}, ${file}_length};" ${output}.h

sed -i "5inamespace ${namespace} {" ${output}.cpp
sed -i '$i}' ${output}.cpp
sed -i '1d' ${output}.cpp

header_filename=$(basename ${output}.h)
sed -i "1i#include \"${header_filename}\"" ${output}.cpp
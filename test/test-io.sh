#!/bin/bash

testfile=$(mktemp)
testfile2=$(mktemp)

dd if=/dev/urandom of=${testfile} bs=1024 count=111024
cat ${testfile} \
|./bin/base64 \
|./bin/zlib \
|./bin/encrypt aes-256-ecb 12345678901234567890123456789012 1234567890123456 \
|./bin/decrypt aes-256-ecb 12345678901234567890123456789012 1234567890123456 \
|./bin/unzlib \
|./bin/unbase64 > ${testfile2}

md5sum ${testfile}
md5sum ${testfile2}

rm ${testfile}
rm ${testfile2}

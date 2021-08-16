#!/bin/bash

dd if=/dev/zero of=./test.bin bs=4k count=25k
md5_1=$(md5sum ./test.bin)
./bin/cvt --input ./test.bin --codec "zlib|hex|zlib|hex|zlib|hex|zlib|hex" > test.bin.codec
./bin/cvt --input ./test.bin.codec --codec "unhex|unzlib|unhex|unzlib|unhex|unzlib|unhex|unzlib" > test.bin
md5_2=$(md5sum ./test.bin)

echo $md5_1
echo $md5_2

dd if=/dev/random of=./test.bin bs=4k count=25k
md5_1=$(md5sum ./test.bin)
./bin/cvt --input ./test.bin --codec "zlib|hex|zlib|hex|zlib|hex|zlib|hex" > test.bin.codec
./bin/cvt --input ./test.bin.codec --codec "unhex|unzlib|unhex|unzlib|unhex|unzlib|unhex|unzlib" > test.bin
md5_2=$(md5sum ./test.bin)

echo $md5_1
echo $md5_2
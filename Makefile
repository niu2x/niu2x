all:
	git add .
	git-clang-format
	git add .
	cmake -S . -B build -DBUILD_SHARED_LIBS=OFF \
	-DOpenSSL_ROOT=/home/niu2x/project/openssl/dist/usr/local	\
	-Dlibuv_DIR=/home/niu2x/project/libuv/build/dist/usr/local/lib/cmake/libuv \
	-DCURL_DIR=/home/niu2x/project/curl/build-linux/dist/usr/local/lib/cmake/CURL \
	-DFFmpeg_ROOT=/home/niu2x/project/ffmpeg/dist/usr/local
	cmake --build build
	make -C build test
	make -C build install DESTDIR=dist

clean:
	rm -rf build

.PHONY: all clean

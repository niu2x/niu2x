all:
	git add .
	git-clang-format
	git add .
	cmake -S . -B build -DBUILD_SHARED_LIBS=OFF -DOpenSSL_ROOT=/home/niu2x/project/openssl/dist/usr/local
	cmake --build build
	make -C build test
	make -C build install DESTDIR=dist

clean:
	rm -rf build

.PHONY: all clean

all:
	git add .
	git-clang-format
	git add .
	cmake -S . -B build -DBUILD_SHARED_LIBS=ON -DOpenSSL_ROOT=/home/niu2x/project/openssl/dist/usr/local
	cmake --build build
	make -C build test

clean:
	rm -rf build

.PHONY: all clean

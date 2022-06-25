all:
	git add .
	git-clang-format
	git add .
	cmake -S . -B build -DOpenSSL_ROOT=/home/niu2x/project/openssl/dist/usr/local
	cmake --build build

clean:
	rm -rf build

.PHONY: all clean

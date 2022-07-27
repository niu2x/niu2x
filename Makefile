all:
	git add .
	git-clang-format
	git add .
	cmake -S . -B build -DBUILD_SHARED_LIBS=ON \
	-Dlibuv_DIR=/home/niu2x/project/libuv/build/dist/usr/local/lib/cmake/libuv \
	-DCURL_DIR=/home/niu2x/project/curl/build-linux/dist/usr/local/lib/cmake/CURL \
	-DOpenSSL_ROOT=/home/niu2x/project/openssl/dist/usr/local
	cmake --build build
	make -C build test
# 	make -C build install DESTDIR=dist

clean:
	rm -rf build


build-android: build-android-armeabi-v7a build-android-arm64-v8a

build-android-armeabi-v7a:
	cmake -S . -B build/android/armeabi-v7a -DBUILD_SHARED_LIBS=ON -DANDROID_PLATFORM=android-21 -DANDROID_ABI=armeabi-v7a \
		-DCMAKE_TOOLCHAIN_FILE=/home/niu2x/Android/Sdk/ndk/21.2.6472646/build/cmake/android.toolchain.cmake
	cmake --build build/android/armeabi-v7a

build-android-arm64-v8a:
	cmake -S . -B build/android/arm64-v8a -DBUILD_SHARED_LIBS=ON -DANDROID_PLATFORM=android-21 -DANDROID_ABI=arm64-v8a \
		-DCMAKE_TOOLCHAIN_FILE=/home/niu2x/Android/Sdk/ndk/21.2.6472646/build/cmake/android.toolchain.cmake
	cmake --build build/android/arm64-v8a

dist-android-armeabi-v7a: build-android-armeabi-v7a
	cd build/android/armeabi-v7a && make install DESTDIR=dist

dist-android-arm64-v8a: build-android-arm64-v8a
	cd build/android/arm64-v8a && make install DESTDIR=dist

dist-android: dist-android-armeabi-v7a dist-android-arm64-v8a
	mkdir -p dist/android/arm64-v8a
	mkdir -p dist/android/armeabi-v7a
	cp -r build/android/arm64-v8a/dist/* dist/android/arm64-v8a/
	cp -r build/android/armeabi-v7a/dist/* dist/android/armeabi-v7a/

dist-linux: build-linux
	cd build/linux && make install DESTDIR=dist
	mkdir -p dist/linux
	cp -r build/linux/dist/* dist/linux/

build-linux:
	cmake -S . -B build/linux -DBUILD_SHARED_LIBS=ON
	cmake --build build/linux

.PHONY: all clean build-android

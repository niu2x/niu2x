add_executable(base64 "base64.cpp")
target_link_libraries(base64 PUBLIC niu2x)

add_executable(unbase64 "unbase64.cpp")
target_link_libraries(unbase64 PUBLIC niu2x)

add_executable(zlib "zlib.cpp")
target_link_libraries(zlib PUBLIC niu2x)

add_executable(unzlib "unzlib.cpp")
target_link_libraries(unzlib PUBLIC niu2x)

add_executable(digest "digest.cpp")
target_link_libraries(digest PUBLIC niu2x)

add_executable(encrypt "encrypt.cpp")
target_link_libraries(encrypt PUBLIC niu2x)

add_executable(decrypt "decrypt.cpp")
target_link_libraries(decrypt PUBLIC niu2x)

add_executable(empty_window "empty_window.cpp")
target_link_libraries(empty_window PUBLIC niu2x)

add_executable(font "font.cpp")
target_link_libraries(font PUBLIC niu2x)

add_executable(test "test.cpp")
target_link_libraries(test PUBLIC niu2x)
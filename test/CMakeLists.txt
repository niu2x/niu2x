add_executable(base64 "base64.cpp")
target_link_libraries(base64 PUBLIC niu2x)

add_executable(unbase64 "unbase64.cpp")
target_link_libraries(unbase64 PUBLIC niu2x)

add_executable(zlib "zlib.cpp")
target_link_libraries(zlib PUBLIC niu2x)

add_executable(unzlib "unzlib.cpp")
target_link_libraries(unzlib PUBLIC niu2x)

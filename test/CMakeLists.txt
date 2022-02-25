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

add_executable(shadow_map "shadow_map.cpp")
target_link_libraries(shadow_map PUBLIC niu2x)

add_executable(stencil "stencil.cpp")
target_link_libraries(stencil PUBLIC niu2x)

add_executable(display_model "display_model.cpp")
target_link_libraries(display_model PUBLIC niu2x)

add_executable(nx "nx.cpp")
target_link_libraries(nx PUBLIC niu2x)
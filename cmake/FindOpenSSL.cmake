find_library(
	OpenSSL_SSL_LIBRARY
	NAMES ssl
	PATH_SUFFIXES lib lib64
)

find_library(
	OpenSSL_Crypto_LIBRARY
	NAMES crypto
	PATH_SUFFIXES lib lib64
)

find_path(OpenSSL_INCLUDE_DIR
	NAMES openssl/ssl.h
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args( OpenSSL
	DEFAULT_MSG
	OpenSSL_SSL_LIBRARY
	OpenSSL_Crypto_LIBRARY
	OpenSSL_INCLUDE_DIR
)

if(NOT TARGET OpenSSL::Crypto)
	add_library(OpenSSL::Crypto UNKNOWN IMPORTED )
	set_target_properties(OpenSSL::Crypto PROPERTIES
		IMPORTED_LOCATION "${OpenSSL_Crypto_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES "${OpenSSL_INCLUDE_DIR}"
		# IMPORTED_LINK_INTERFACE_LANGUAGES "C"
	)
	# install(FILES ${OpenSSL_Crypto_LIBRARY} TYPE LIB)
	# install(DIRECTORY ${OpenSSL_INCLUDE_DIR}/ TYPE INCLUDE)
endif()

if(NOT TARGET OpenSSL::SSL)
	add_library(OpenSSL::SSL UNKNOWN IMPORTED )
	set_target_properties(OpenSSL::SSL PROPERTIES
		IMPORTED_LOCATION "${OpenSSL_SSL_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES "${OpenSSL_INCLUDE_DIR}"
		# IMPORTED_LINK_INTERFACE_LANGUAGES "C"
	)
	# install(FILES ${OpenSSL_SSL_LIBRARY} TYPE LIB)
	# install(DIRECTORY ${OpenSSL_INCLUDE_DIR}/ TYPE INCLUDE)
endif()

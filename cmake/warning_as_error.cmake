# enable warning_as_error in current dir and all subdirs.
function(warning_as_error_enable)
	if (MSVC)
	    # warning level 4 and all warnings as errors
	    add_compile_options(/W4 /WX)
	else()
	    # lots of warnings and all warnings as errors
	    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
	endif()
endfunction()

# disable warning_as_error in current dir and all subdirs.
function(warning_as_error_disable)
	if (MSVC)
	else()
	    add_compile_options(-Wno-all -Wno-extra -Wno-pedantic -Wno-error)
	endif()
endfunction()




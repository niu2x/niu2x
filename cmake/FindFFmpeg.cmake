list(APPEND components 
	swscale
	avcodec
	swresample
	avutil
	avformat
	avfilter
	avdevice
)

foreach(item ${components})
	find_library(
		FFmpeg_${item}_LIBRARY
		NAMES ${item}
		PATH_SUFFIXES lib
	)	
endforeach()

find_path(FFmpeg_INCLUDE_DIR
	NAMES libswscale/version.h
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args( FFmpeg
	DEFAULT_MSG
	FFmpeg_swscale_LIBRARY
	FFmpeg_avdevice_LIBRARY
	FFmpeg_avfilter_LIBRARY
	FFmpeg_avformat_LIBRARY
	FFmpeg_avutil_LIBRARY
	FFmpeg_avcodec_LIBRARY
	FFmpeg_swresample_LIBRARY
	FFmpeg_INCLUDE_DIR
)

foreach(item ${components})
	if(NOT TARGET FFmpeg::${item})
		add_library(FFmpeg::${item} UNKNOWN IMPORTED )
		set_target_properties(FFmpeg::${item} PROPERTIES
			IMPORTED_LOCATION "${FFmpeg_${item}_LIBRARY}"
			INTERFACE_INCLUDE_DIRECTORIES "${FFmpeg_INCLUDE_DIR}"
		)
	endif()
endforeach()
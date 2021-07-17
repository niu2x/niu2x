function(dependent_prebuilt)
    set(options "")
    set(oneValueArgs TARGET)
    set(multiValueArgs LIBS)
    cmake_parse_arguments(P
        "${options}" 
        "${oneValueArgs}"
        "${multiValueArgs}" 
        ${ARGN} 
    )

    set(depdir ${CMAKE_SOURCE_DIR}/dependencies)
    foreach(libname IN LISTS P_LIBS)
        target_include_directories(${P_TARGET} PRIVATE 
            ${depdir}/${libname}/include
        )   

        string(TOLOWER ${CMAKE_SYSTEM_NAME} system_name)

        set(libdir ${depdir}/${libname}/lib/${system_name}/${CMAKE_SYSTEM_PROCESSOR})
        target_link_directories(${P_TARGET} PRIVATE 
            ${libdir}
        )

        file(GLOB_RECURSE libfiles PATTERN "${libdir}/*") 
        target_link_libraries(${P_TARGET} PRIVATE
            # -Wl,--whole-archive 
            ${libfiles} 
            # -Wl,--no-whole-archive
        )
    endforeach()
endfunction()
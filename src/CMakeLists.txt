qt_add_library(GenO)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers(GenO
    PUBLIC
        global.h config.h
        objectinterface.h object.h
    PRIVATE
        debug_p.h
        object_p.h
)

target_sources(GenO
    PRIVATE
        debug.cpp
        objectinterface.cpp object.cpp
)

target_include_directories(GenO PUBLIC $<INSTALL_INTERFACE:include>)

target_link_libraries(GenO PUBLIC Qt::Core)

if (BUILD_SHARED_LIBS)
    set(GENO_BUILD_TYPE SHARED)
else()
    set(GENO_BUILD_TYPE STATIC)
endif()

target_compile_definitions(GenO
    PUBLIC
        GENO_LIB GENO_${GENO_BUILD_TYPE}
    PRIVATE
        GENO_BUILD
)

generate_target_headers(GenO FOLDER GenO)

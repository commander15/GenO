qt_add_library(GenO)

set_target_properties(GenO PROPERTIES
    OUTPUT_NAME GenO
    VERSION     ${PROJECT_VERSION}
)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers(GenO
    PUBLIC
        geno.h
        global.h config.h
        objectinterface.h object.h
        serialization.h
        jsonserialization.h
    PRIVATE
        debug_p.h
        object_p.h
)

target_sources(GenO
    PRIVATE
        debug.cpp
        objectinterface.cpp object.cpp
        serialization.cpp
        jsonserialization.cpp
)

target_include_directories(GenO 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/GenO>
)

target_compile_definitions(GenO
    PUBLIC
        $<IF:$<STREQUAL:$<TARGET_PROPERTY:TYPE>,STATIC_LIBRARY>,GENO_STATIC,GENO_SHARED>
    INTERFACE
        GENO_LIB
    PRIVATE
        GENO_BUILD
)

target_link_libraries(GenO PUBLIC Qt::Core)

install(TARGETS GenO
    EXPORT GenOTargets
    ARCHIVE        DESTINATION lib
    LIBRARY        DESTINATION lib
    RUNTIME        DESTINATION bin
    PUBLIC_HEADER  DESTINATION include/GenO
    #PRIVATE_HEADER DESTINATION include/GenO/private
)

generate_target_headers(GenO FOLDER GenO)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    GenOConfig.cmake.in
    ${CMAKE_CURRENT_SOURCE_DIR}/GenOConfig.cmake
    INSTALL_DESTINATION lib/cmake/GenO
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/GenOConfigVersion.cmake
    VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion
    ARCH_INDEPENDENT

)

add_subdirectory(lib)

install(FILES GenOConfig.cmake GenOConfigVersion.cmake
    DESTINATION lib/cmake/GenO
)

install(EXPORT GenOTargets
    NAMESPACE GenO::
    DESTINATION lib/cmake/GenO
)

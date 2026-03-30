
add_fusion_target(FusionPlatform STATIC
    ALIAS       Fusion::Platform
    FOLDER      "Modules"
    FILES_CMAKE
        PRIVATE fusionplatform_private_files.cmake
        PUBLIC  fusionplatform_public_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PCHHEADER
        PRIVATE Include/Fusion/Platform.h
    BUILD_DEPENDENCIES
        PUBLIC
            Fusion::Core
)

if(FUSION_BUILD_TESTS)
    add_subdirectory(Tests)
endif()

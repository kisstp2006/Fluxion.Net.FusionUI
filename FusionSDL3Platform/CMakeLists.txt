
add_fusion_target(FusionSDL3Platform STATIC
    ALIAS       Fusion::SDL3Platform
    FOLDER      "Modules"
    FILES_CMAKE
        PRIVATE fusionsdl3platform_private_files.cmake
        PUBLIC  fusionsdl3platform_public_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PCHHEADER
        PRIVATE Include/Fusion/SDL3Platform.h
    BUILD_DEPENDENCIES
        PUBLIC
            Fusion::Core
            Fusion::Platform
            SDL3::SDL3
)


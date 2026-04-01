
add_fusion_target(FusionRHI STATIC
    ALIAS       Fusion::RHI
    FOLDER      "Modules"
    FILES_CMAKE
        PRIVATE fusionrhi_private_files.cmake
        PUBLIC  fusionrhi_public_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PCHHEADER
        PRIVATE Include/Fusion/RHI.h
    BUILD_DEPENDENCIES
        PUBLIC
            Fusion::Core
            Fusion::Platform
            Fusion::ShaderLibrary
)

if(FUSION_BUILD_TESTS)
    add_subdirectory(Tests)
endif()


add_fusion_target(FusionCore STATIC
    ALIAS       Fusion::Core
    FOLDER      "FusionCore"
    FILES_CMAKE
        PRIVATE fusioncore_private_files.cmake
        PUBLIC  fusioncore_public_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PCHHEADER
        PRIVATE Include/Fusion/FusionCore.h
    BUILD_DEPENDENCIES
        PUBLIC
            cpptrace::cpptrace
)

if(FUSION_BUILD_TESTS)
    add_subdirectory(Tests)
endif()

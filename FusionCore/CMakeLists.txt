
add_fusion_target(FusionCore STATIC
    ALIAS       Fusion::Core
    FOLDER      "Modules"
    FILES_CMAKE
        PRIVATE fusioncore_private_files.cmake
        PUBLIC  fusioncore_public_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    COMPILE_DEFINITIONS
        PUBLIC FUSION_ENABLE_TRACY=$<BOOL:${FUSION_ENABLE_TRACY}>
    PCHHEADER
        PRIVATE Include/Fusion/Core.h
    BUILD_DEPENDENCIES
        PRIVATE
            xxHash::xxhash
        PUBLIC
            cpptrace::cpptrace
            $<$<BOOL:${FUSION_ENABLE_TRACY}>:Tracy::TracyClient>
)

if(FUSION_BUILD_TESTS)
    add_subdirectory(Tests)
endif()

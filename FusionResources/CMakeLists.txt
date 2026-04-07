add_fusion_target(FusionResources STATIC
    ALIAS       Fusion::Resources
    FOLDER      "Modules"
    FILES_CMAKE
        PRIVATE fusionresources_private_files.cmake
        PUBLIC  fusionresources_public_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    BUILD_DEPENDENCIES
        PUBLIC
            Fusion::Core
)

fusion_embed_resources_dir(
    TARGET    FusionResources
    DIR       ${CMAKE_CURRENT_SOURCE_DIR}/Resources
    BASE_PATH "/"
)

fusion_finalize_resource_registry(FusionResources)

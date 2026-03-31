
add_fusion_target(FusionVulkanRHI STATIC
    ALIAS       Fusion::VulkanRHI
    FOLDER      "Modules"
    FILES_CMAKE
        PRIVATE fusionvulkanrhi_private_files.cmake
        PUBLIC  fusionvulkanrhi_public_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PCHHEADER
        PRIVATE Include/Fusion/VulkanRHI.h
    BUILD_DEPENDENCIES
        PUBLIC
            Fusion::Core
            Fusion::Platform
            Fusion::RHI
            Vulkan::Vulkan
)


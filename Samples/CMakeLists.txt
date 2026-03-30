
add_fusion_target(FusionSamples CONSOLEAPP
    ALIAS       Fusion::Samples
    FOLDER      "Samples"
    FILES_CMAKE
        PRIVATE samples_private_files.cmake
    BUILD_DEPENDENCIES
        PUBLIC
            Fusion::Core
            Fusion::Platform
            Fusion::SDL3Platform
            Fusion::RHI
            Fusion::VulkanRHI
            Fusion::Widgets
)

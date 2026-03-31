
file(GLOB_RECURSE SAMPLES_SOURCES CONFIGURE_DEPENDS
    Source/*.cpp
    Source/*.h
)

add_executable(FusionSamples ${SAMPLES_SOURCES})
add_executable(Fusion::Samples ALIAS FusionSamples)

target_link_libraries(FusionSamples
    PUBLIC
        Fusion::Core
        Fusion::Platform
        Fusion::SDL3Platform
        Fusion::RHI
        Fusion::VulkanRHI
        Fusion::Widgets
)

set_target_properties(FusionSamples PROPERTIES FOLDER "Samples")

fusion_configure_target(FusionSamples)

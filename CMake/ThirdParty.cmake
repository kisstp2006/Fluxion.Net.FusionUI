include_guard(GLOBAL)

option(FUSION_USE_SDL3     "Build the SDL3 windowing backend"  ON)
option(FUSION_USE_VULKAN   "Enable Vulkan rendering backend"   ON)

# Required dependencies
include(CMake/ThirdParty/CppTrace.cmake)
include(CMake/ThirdParty/xxHash.cmake)

if(FUSION_USE_SDL3)
    include(CMake/ThirdParty/SDL3.cmake)
endif()

if(FUSION_USE_VULKAN)
    include(CMake/ThirdParty/Vulkan.cmake)
endif()

include(CMake/ThirdParty/FreeType.cmake)

if(FUSION_ENABLE_TRACY)
    include(CMake/ThirdParty/Tracy.cmake)
endif()

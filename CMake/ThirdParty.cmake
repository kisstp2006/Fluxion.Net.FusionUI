include_guard(GLOBAL)

option(FUSION_USE_SDL3     "Build the SDL3 windowing backend"  ON)
option(FUSION_USE_FREETYPE "Use FreeType for font rendering"   ON)

# Required dependencies
include(CMake/ThirdParty/CppTrace.cmake)

if(FUSION_USE_SDL3)
    include(CMake/ThirdParty/SDL3.cmake)
endif()

if(FUSION_USE_FREETYPE)
    include(CMake/ThirdParty/FreeType.cmake)
endif()

if(FUSION_ENABLE_TRACY)
    include(CMake/ThirdParty/Tracy.cmake)
endif()

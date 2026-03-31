include_guard(GLOBAL)

option(FUSION_USE_SDL3     "Build the SDL3 windowing backend"  ON)
option(FUSION_USE_VULKAN   "Enable Vulkan rendering backend"   ON)

# ---------------------------------------------------------------------------
# fusion_configure_third_party_target(<target>)
#
# Apply minimal build settings to a third-party target fetched via FetchContent.
# Only call this on targets Fusion fetched itself — never on pre-existing targets.
# ---------------------------------------------------------------------------
function(fusion_configure_third_party_target target)
    if(NOT TARGET ${target})
        return()
    endif()

    if(FUSION_COMPILER_MSVC)
        target_compile_options(${target} PRIVATE /MP)
    endif()

    if(PROJECT_IS_TOP_LEVEL)
        set_target_properties(${target} PROPERTIES FOLDER "ThirdParty")
    endif()
endfunction()

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

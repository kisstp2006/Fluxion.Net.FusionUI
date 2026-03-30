include_guard(GLOBAL)

if(TARGET Vulkan::Vulkan)
    return()
endif()

find_package(Vulkan REQUIRED)

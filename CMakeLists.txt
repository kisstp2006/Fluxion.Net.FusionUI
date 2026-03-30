cmake_minimum_required(VERSION 3.21)

project(FusionUI
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CMake/FusionPlatform.cmake)

if(PROJECT_IS_TOP_LEVEL)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    option(FUSION_BUILD_TESTS    "Build Fusion tests"              ON)
    option(FUSION_ENABLE_TRACY   "Enable Tracy profiler integration" OFF)
endif()



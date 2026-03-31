include_guard(GLOBAL)

if(TARGET GoogleTest)
    return()
endif()

find_package(GTest QUIET)
if(GTest_FOUND)
    add_library(GoogleTest INTERFACE IMPORTED)
    target_link_libraries(GoogleTest INTERFACE GTest::gtest_main)
    return()
endif()

message(STATUS "[Fusion] GoogleTest not found, fetching via FetchContent...")

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
    GIT_SHALLOW    TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Force GTest to build as a static library regardless of BUILD_SHARED_LIBS
set(_prev_build_shared_libs ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(googletest)
set(BUILD_SHARED_LIBS ${_prev_build_shared_libs})

add_library(GoogleTest INTERFACE IMPORTED)
target_link_libraries(GoogleTest INTERFACE gtest_main)

fusion_configure_third_party_target(gmock)
fusion_configure_third_party_target(gmock_main)
fusion_configure_third_party_target(gtest)
fusion_configure_third_party_target(gtest_main)

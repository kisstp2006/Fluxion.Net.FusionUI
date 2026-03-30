include_guard(GLOBAL)

if(TARGET SDL3::SDL3)
    return()
endif()

find_package(SDL3 QUIET)
if(SDL3_FOUND)
    return()
endif()

message(STATUS "[Fusion] SDL3 not found, fetching via FetchContent...")

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.0
    GIT_SHALLOW    TRUE
)

set(SDL_SHARED  OFF CACHE BOOL "" FORCE)
set(SDL_STATIC  ON  CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3)

fusion_configure_third_party_target(SDL3-static)
fusion_configure_third_party_target(SDL3_test)
fusion_configure_third_party_target(SDL_uclibc)

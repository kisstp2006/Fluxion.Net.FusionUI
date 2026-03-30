include_guard(GLOBAL)

if(TARGET xxHash::xxhash)
    return()
endif()

find_package(xxHash QUIET)
if(xxHash_FOUND)
    return()
endif()

message(STATUS "[Fusion] xxHash not found, fetching via FetchContent...")

FetchContent_Declare(
    xxHash
    GIT_REPOSITORY https://github.com/Cyan4973/xxHash.git
    GIT_TAG        v0.8.3
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  cmake_unofficial
)

set(XXHASH_BUILD_XXHSUM OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(xxHash)

fusion_configure_third_party_target(xxhash)

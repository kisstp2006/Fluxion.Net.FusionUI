include_guard(GLOBAL)

if(TARGET Tracy::TracyClient)
    return()
endif()

find_package(Tracy QUIET)
if(Tracy_FOUND)
    return()
endif()

message(STATUS "[Fusion] Tracy not found, fetching via FetchContent...")

FetchContent_Declare(
    Tracy
    GIT_REPOSITORY https://github.com/wolfpld/tracy.git
    GIT_TAG        v0.11.1
    GIT_SHALLOW    TRUE
)

set(TRACY_ENABLE  ${FUSION_ENABLE_TRACY} CACHE BOOL "" FORCE)
set(TRACY_ON_DEMAND ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(Tracy)

fusion_configure_third_party_target(TracyClient)

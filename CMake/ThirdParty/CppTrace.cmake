include_guard(GLOBAL)

if(TARGET cpptrace::cpptrace)
    return()
endif()

find_package(cpptrace QUIET)
if(cpptrace_FOUND)
    return()
endif()

message(STATUS "[Fusion] cpptrace not found, fetching via FetchContent...")

FetchContent_Declare(
    cpptrace
    GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
    GIT_TAG        v0.7.5
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(cpptrace)

fusion_configure_third_party_target(cpptrace-lib)

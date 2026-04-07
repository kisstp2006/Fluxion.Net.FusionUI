include_guard(GLOBAL)

if(TARGET Freetype::Freetype)
    return()
endif()

find_package(Freetype QUIET)
if(Freetype_FOUND)
    return()
endif()

message(STATUS "[Fusion] FreeType not found, fetching via FetchContent...")

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG        VER-2-14-0
    GIT_SHALLOW    TRUE
)

set(FT_DISABLE_ZLIB   ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2  ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG    ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(freetype)

fusion_configure_third_party_target(freetype)

# FetchContent builds freetype as "freetype", normalize to the find_package target name
if(NOT TARGET Freetype::Freetype)
    add_library(Freetype::Freetype ALIAS freetype)
endif()

file(GLOB_RECURSE FILES CONFIGURE_DEPENDS
    Source/*.cpp
    Source/*.h
)

fusion_filter_platform_files(FILES)

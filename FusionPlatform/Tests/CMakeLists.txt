file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_fusion_test(FusionPlatformTests
    TARGET  FusionPlatform
    FOLDER  "Tests"
    SOURCES ${TEST_SOURCES}
)

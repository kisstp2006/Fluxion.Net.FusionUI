file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_fusion_test(FusionCoreTests
    TARGET  FusionCore
    FOLDER  "FusionCore/Tests"
    SOURCES ${TEST_SOURCES}
)

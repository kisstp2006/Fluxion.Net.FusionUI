file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_fusion_test(FusionRHITests
    TARGET  FusionRHI
    FOLDER  "Tests"
    SOURCES ${TEST_SOURCES}
)

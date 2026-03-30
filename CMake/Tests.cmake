include_guard(GLOBAL)

if(NOT FUSION_BUILD_TESTS)
    return()
endif()

enable_testing()

include(CMake/ThirdParty/GoogleTest.cmake)

# ---------------------------------------------------------------------------
# add_fusion_test(<name>
#   TARGET     <fusion-target-under-test>
#   FOLDER     <ide-folder>
#   SOURCES    <file> ...     (optional — globs Tests/*.cpp|h if omitted)
#   BUILD_DEPENDENCIES
#       PRIVATE|PUBLIC|INTERFACE <dep> ...
# )
# ---------------------------------------------------------------------------
function(add_fusion_test NAME)
    set(oneValueArgs   TARGET FOLDER)
    set(multiValueArgs SOURCES BUILD_DEPENDENCIES)

    cmake_parse_arguments(add_fusion_test "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT add_fusion_test_TARGET)
        message(FATAL_ERROR "[Fusion] add_fusion_test(${NAME}): TARGET is required")
    endif()

    if(NOT add_fusion_test_SOURCES)
        message(FATAL_ERROR "[Fusion] add_fusion_test(${NAME}): SOURCES is required")
    endif()

    set(TEST_SOURCES ${add_fusion_test_SOURCES})

    add_executable(${NAME} ${TEST_SOURCES})

    fusion_configure_target(${NAME})
    fusion_group_sources_by_folder(${NAME})

    if(FUSION_PLATFORM_WINDOWS)
        set_property(TARGET ${NAME} PROPERTY
            MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug>:ProgramDatabase>"
        )
    endif()

    if(add_fusion_test_FOLDER AND PROJECT_IS_TOP_LEVEL)
        set_target_properties(${NAME} PROPERTIES FOLDER "${add_fusion_test_FOLDER}")
    endif()

    # Link GoogleTest + the target under test
    target_link_libraries(${NAME} PRIVATE GoogleTest ${add_fusion_test_TARGET})

    # Inherit public include directories from the target under test
    get_target_property(target_includes ${add_fusion_test_TARGET} INCLUDE_DIRECTORIES)
    if(target_includes)
        target_include_directories(${NAME} PRIVATE ${target_includes})
    endif()

    # BUILD_DEPENDENCIES
    set(multiValueArgs PRIVATE PUBLIC INTERFACE)
    cmake_parse_arguments(add_fusion_test_BUILD_DEPENDENCIES "" "" "${multiValueArgs}" ${add_fusion_test_BUILD_DEPENDENCIES})

    if(add_fusion_test_BUILD_DEPENDENCIES_PRIVATE OR
       add_fusion_test_BUILD_DEPENDENCIES_PUBLIC  OR
       add_fusion_test_BUILD_DEPENDENCIES_INTERFACE)
        target_link_libraries(${NAME}
            PRIVATE   ${add_fusion_test_BUILD_DEPENDENCIES_PRIVATE}
            PUBLIC    ${add_fusion_test_BUILD_DEPENDENCIES_PUBLIC}
            INTERFACE ${add_fusion_test_BUILD_DEPENDENCIES_INTERFACE}
        )
    endif()

    add_test(NAME ${NAME} COMMAND ${NAME})

endfunction()

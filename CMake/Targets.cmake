include_guard(GLOBAL)

set(TARGET_TYPE_STATIC_IS_LIBRARY     TRUE)
set(TARGET_TYPE_SHARED_IS_LIBRARY     TRUE)
set(TARGET_TYPE_CONSOLEAPP_IS_EXECUTABLE TRUE)
set(TARGET_TYPE_GUIAPP_IS_EXECUTABLE     TRUE)

set(TARGET_TYPE_STATIC_IS_STATICLIB   TRUE)
set(TARGET_TYPE_SHARED_IS_SHAREDLIB   TRUE)

# ---------------------------------------------------------------------------
# fusion_group_sources_by_folder(<target>)
# Groups source files in the IDE by their directory structure.
# ---------------------------------------------------------------------------
function(fusion_group_sources_by_folder target)
    set(last_dir "")
    set(files "")

    get_target_property(sources ${target} SOURCES)

    foreach(file ${sources})
        if(IS_ABSOLUTE ${file})
            file(RELATIVE_PATH relative_file "${CMAKE_CURRENT_SOURCE_DIR}" ${file})
        else()
            set(relative_file ${file})
        endif()

        get_filename_component(dir "${relative_file}" PATH)

        if(NOT "${dir}" STREQUAL "${last_dir}")
            if(files)
                source_group("${last_dir}" FILES ${files})
            endif()
            set(files "")
        endif()

        list(APPEND files ${file})
        set(last_dir "${dir}")
    endforeach()

    if(files)
        source_group("${last_dir}" FILES ${files})
    endif()
endfunction()

# ---------------------------------------------------------------------------
# add_fusion_target(<name> <type>
#   FILES_CMAKE
#       PRIVATE <file.cmake> ...
#       PUBLIC  <file.cmake> ...
#   INCLUDE_DIRECTORIES
#       PRIVATE|PUBLIC|INTERFACE <dir> ...
#   BUILD_DEPENDENCIES
#       PRIVATE|PUBLIC|INTERFACE <target> ...
#   COMPILE_DEFINITIONS
#       PRIVATE|PUBLIC|INTERFACE <def> ...
#   PCHHEADER
#       PRIVATE|PUBLIC|INTERFACE <header> | REUSE_FROM <target>
#   FOLDER        <ide-folder>
#   NAMESPACE     <ns>
#   OUTPUT_DIRECTORY <subdir>
#   VS_STARTUP_PROJECT
# )
#
# Valid types: STATIC  SHARED  CONSOLEAPP  GUIAPP
# ---------------------------------------------------------------------------
function(add_fusion_target NAME TARGET_TYPE)

    set(options        VS_STARTUP_PROJECT)
    set(oneValueArgs   FOLDER NAMESPACE ALIAS OUTPUT_DIRECTORY)
    set(multiValueArgs FILES_CMAKE INCLUDE_DIRECTORIES BUILD_DEPENDENCIES COMPILE_DEFINITIONS PCHHEADER APPLE_FRAMEWORKS)

    cmake_parse_arguments(add_fusion_target "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT add_fusion_target_FILES_CMAKE)
        message(FATAL_ERROR "[Fusion] add_fusion_target(${NAME}): FILES_CMAKE is required")
    endif()

    # ------------------------------------------------------------------
    # Collect sources from FILES_CMAKE lists
    # ------------------------------------------------------------------
    set(PRIVATE_FILES "")
    set(PUBLIC_FILES  "")
    set(CMAKE_FILES   "")

    set(multiValueArgs PRIVATE PUBLIC)
    cmake_parse_arguments(add_fusion_target_FILES_CMAKE "" "" "${multiValueArgs}" ${add_fusion_target_FILES_CMAKE})

    foreach(cmake_file ${add_fusion_target_FILES_CMAKE_PRIVATE})
        include(${cmake_file})
        list(APPEND CMAKE_FILES   ${cmake_file})
        list(APPEND PRIVATE_FILES ${FILES})
    endforeach()

    foreach(cmake_file ${add_fusion_target_FILES_CMAKE_PUBLIC})
        include(${cmake_file})
        list(APPEND CMAKE_FILES  ${cmake_file})
        list(APPEND PUBLIC_FILES ${FILES})
    endforeach()

    set(ALL_SOURCES ${PRIVATE_FILES} ${PUBLIC_FILES} ${CMAKE_FILES})

    # ------------------------------------------------------------------
    # Create the target
    # ------------------------------------------------------------------
    if(TARGET_TYPE_${TARGET_TYPE}_IS_LIBRARY)
        add_library(${NAME} ${TARGET_TYPE} ${ALL_SOURCES})

        if(add_fusion_target_ALIAS)
            add_library("${add_fusion_target_ALIAS}" ALIAS ${NAME})
        elseif(add_fusion_target_NAMESPACE)
            add_library("${add_fusion_target_NAMESPACE}::${NAME}" ALIAS ${NAME})
        endif()

    elseif(TARGET_TYPE_${TARGET_TYPE}_IS_EXECUTABLE)
        add_executable(${NAME} ${ALL_SOURCES})

        if(add_fusion_target_VS_STARTUP_PROJECT)
            set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${NAME})
        endif()

    else()
        message(FATAL_ERROR "[Fusion] add_fusion_target(${NAME}): unknown TARGET_TYPE '${TARGET_TYPE}'")
    endif()

    # ------------------------------------------------------------------
    # Compile settings (flags, platform/version defines)
    # ------------------------------------------------------------------
    fusion_configure_target(${NAME})
    fusion_group_sources_by_folder(${NAME})

    if(FUSION_PLATFORM_WINDOWS)
        set_property(TARGET ${NAME} PROPERTY
            MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug>:ProgramDatabase>"
        )
    endif()

    # ------------------------------------------------------------------
    # FOLDER
    # ------------------------------------------------------------------
    if(add_fusion_target_FOLDER AND PROJECT_IS_TOP_LEVEL)
        set_target_properties(${NAME} PROPERTIES FOLDER "${add_fusion_target_FOLDER}")
    endif()

    # ------------------------------------------------------------------
    # OUTPUT_DIRECTORY
    # ------------------------------------------------------------------
    if(add_fusion_target_OUTPUT_DIRECTORY)
        set_target_properties(${NAME} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${add_fusion_target_OUTPUT_DIRECTORY}"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${add_fusion_target_OUTPUT_DIRECTORY}"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${add_fusion_target_OUTPUT_DIRECTORY}"
        )
    endif()

    # ------------------------------------------------------------------
    # INCLUDE_DIRECTORIES
    # ------------------------------------------------------------------
    set(multiValueArgs PRIVATE PUBLIC INTERFACE)
    cmake_parse_arguments(add_fusion_target_INCLUDE_DIRECTORIES "" "" "${multiValueArgs}" ${add_fusion_target_INCLUDE_DIRECTORIES})

    target_include_directories(${NAME}
        PRIVATE   ${add_fusion_target_INCLUDE_DIRECTORIES_PRIVATE}
        PUBLIC    ${add_fusion_target_INCLUDE_DIRECTORIES_PUBLIC}
        INTERFACE ${add_fusion_target_INCLUDE_DIRECTORIES_INTERFACE}
    )

    # ------------------------------------------------------------------
    # BUILD_DEPENDENCIES
    # ------------------------------------------------------------------
    set(multiValueArgs PRIVATE PUBLIC INTERFACE)
    cmake_parse_arguments(add_fusion_target_BUILD_DEPENDENCIES "" "" "${multiValueArgs}" ${add_fusion_target_BUILD_DEPENDENCIES})

    if(add_fusion_target_BUILD_DEPENDENCIES_PRIVATE OR
       add_fusion_target_BUILD_DEPENDENCIES_PUBLIC  OR
       add_fusion_target_BUILD_DEPENDENCIES_INTERFACE)
        target_link_libraries(${NAME}
            PRIVATE   ${add_fusion_target_BUILD_DEPENDENCIES_PRIVATE}
            PUBLIC    ${add_fusion_target_BUILD_DEPENDENCIES_PUBLIC}
            INTERFACE ${add_fusion_target_BUILD_DEPENDENCIES_INTERFACE}
        )
    endif()

    # ------------------------------------------------------------------
    # COMPILE_DEFINITIONS  +  API export macro
    # ------------------------------------------------------------------
    set(multiValueArgs PRIVATE PUBLIC INTERFACE)
    cmake_parse_arguments(add_fusion_target_COMPILE_DEFINITIONS "" "" "${multiValueArgs}" ${add_fusion_target_COMPILE_DEFINITIONS})

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        list(APPEND add_fusion_target_COMPILE_DEFINITIONS_PUBLIC "FUSION_64BIT=1")
    endif()

    if(FUSION_ENABLE_EXCEPTIONS)
        list(APPEND add_fusion_target_COMPILE_DEFINITIONS_PUBLIC "FUSION_EXCEPTIONS=1")
    endif()
    
    if(FUSION_USE_SDL3)
        list(APPEND add_fusion_target_COMPILE_DEFINITIONS_PUBLIC "FUSION_USE_SDL3=1")
    endif()
    
    if(FUSION_USE_VULKAN)
        list(APPEND add_fusion_target_COMPILE_DEFINITIONS_PUBLIC "FUSION_USE_VULKAN=1")
    endif()

    if(FUSION_TRAIT_VULKAN_SUPPORTED)
        list(APPEND add_fusion_target_COMPILE_DEFINITIONS_PUBLIC "FUSION_TRAIT_VULKAN_SUPPORTED=1")
    endif()
    

    string(TOUPPER ${NAME} NAME_UPPER)
    if(FUSION_PLATFORM_WINDOWS AND TARGET_TYPE_${TARGET_TYPE}_IS_SHAREDLIB)
        list(APPEND add_fusion_target_COMPILE_DEFINITIONS_PRIVATE   "${NAME_UPPER}_API=__declspec(dllexport)")
        list(APPEND add_fusion_target_COMPILE_DEFINITIONS_INTERFACE "${NAME_UPPER}_API=__declspec(dllimport)")
    else()
        list(APPEND add_fusion_target_COMPILE_DEFINITIONS_PUBLIC "${NAME_UPPER}_API=")
    endif()

    target_compile_definitions(${NAME}
        PRIVATE   ${add_fusion_target_COMPILE_DEFINITIONS_PRIVATE}
        PUBLIC    ${add_fusion_target_COMPILE_DEFINITIONS_PUBLIC}
        INTERFACE ${add_fusion_target_COMPILE_DEFINITIONS_INTERFACE}
    )

    # ------------------------------------------------------------------
    # PCHHEADER
    # ------------------------------------------------------------------
    if(add_fusion_target_PCHHEADER)
        set(oneValueArgs REUSE_FROM)
        set(multiValueArgs PRIVATE PUBLIC INTERFACE)
        cmake_parse_arguments(add_fusion_target_PCHHEADER "" "${oneValueArgs}" "${multiValueArgs}" ${add_fusion_target_PCHHEADER})

        if(add_fusion_target_PCHHEADER_REUSE_FROM)
            target_precompile_headers(${NAME} REUSE_FROM ${add_fusion_target_PCHHEADER_REUSE_FROM})
        else()
            target_precompile_headers(${NAME}
                PRIVATE   ${add_fusion_target_PCHHEADER_PRIVATE}
                PUBLIC    ${add_fusion_target_PCHHEADER_PUBLIC}
                INTERFACE ${add_fusion_target_PCHHEADER_INTERFACE}
            )
        endif()
    endif()

    # ------------------------------------------------------------------
    # APPLE_FRAMEWORKS  (macOS/iOS only)
    # ------------------------------------------------------------------
    if(APPLE AND add_fusion_target_APPLE_FRAMEWORKS)
        set(multiValueArgs PRIVATE PUBLIC INTERFACE)
        cmake_parse_arguments(add_fusion_target_APPLE_FRAMEWORKS "" "" "${multiValueArgs}" ${add_fusion_target_APPLE_FRAMEWORKS})

        set(_fw_private "")
        set(_fw_public "")
        set(_fw_interface "")
        foreach(fw ${add_fusion_target_APPLE_FRAMEWORKS_PRIVATE})
            list(APPEND _fw_private "-framework ${fw}")
        endforeach()
        foreach(fw ${add_fusion_target_APPLE_FRAMEWORKS_PUBLIC})
            list(APPEND _fw_public "-framework ${fw}")
        endforeach()
        foreach(fw ${add_fusion_target_APPLE_FRAMEWORKS_INTERFACE})
            list(APPEND _fw_interface "-framework ${fw}")
        endforeach()

        target_link_libraries(${NAME}
            PRIVATE   ${_fw_private}
            PUBLIC    ${_fw_public}
            INTERFACE ${_fw_interface}
        )
    endif()

endfunction()

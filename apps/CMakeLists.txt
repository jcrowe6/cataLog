get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../" ABSOLUTE)

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
        "${cataLog_SOURCE_DIR}/apps/*.h"
        "${cataLog_SOURCE_DIR}/apps/*.hpp"
        "${cataLog_SOURCE_DIR}/apps/*.cc"
        "${cataLog_SOURCE_DIR}/apps/*.cpp")


include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

ci_make_app(
        APP_NAME    cinder-cataLog
        CINDER_PATH ${CINDER_PATH}
        SOURCES     ${SOURCE_LIST}
        LIBRARIES
        BLOCKS
)

target_compile_features(cinder-cataLog PRIVATE cxx_std_14)

# Cross-platform compiler lints
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
        OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    target_compile_options(cinder-cataLog PRIVATE
            -Wall
            -Wextra
            -Wswitch
            -Wconversion
            -Wparentheses
            -Wfloat-equal
            -Wzero-as-null-pointer-constant
            -Wpedantic
            -pedantic
            -pedantic-errors)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    cmake_policy(SET CMP0015 NEW)
    target_compile_options(cinder-cataLog PRIVATE
            /W3)
endif ()

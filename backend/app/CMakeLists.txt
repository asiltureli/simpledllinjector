cmake_minimum_required(VERSION 3.20)

project(Injectorx86)
set(TARGET_NAME Injectorx86_headless)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMPILING_X86 ON)
set(FILE_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../bin)
# Create a new cmake path to not override the one from main project
set(MY_CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) 

include(${MY_CMAKE_MODULE_PATH}/ManifestEmbedding.cmake)

add_executable(${TARGET_NAME} console.cpp)

set_target_properties(${TARGET_NAME} PROPERTIES
        COMPILE_OPTIONS "/arch:IA32"
        LINK_FLAGS "/MACHINE:X86"
    )

add_manifest(${TARGET_NAME})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src/processmanager ${CMAKE_BINARY_DIR}/ProcessManagerx86)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/processmanager )

target_link_libraries(${TARGET_NAME} PRIVATE ProcessManagerx86)

add_custom_command(TARGET Injectorx86_headless POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${FILE_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Injectorx86_headless> ${FILE_OUTPUT_DIR}/
        COMMENT "Copying output file to ${FILE_OUTPUT_DIR}"
    )

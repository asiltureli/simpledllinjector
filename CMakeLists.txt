cmake_minimum_required(VERSION 3.5)

project(SimpleDLLInjector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(MY_CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(FILE_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include(ManifestEmbedding)

set(PROJECT_SOURCES
        main.cpp)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SimpleDLLInjector
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES})
else()
        add_executable(SimpleDLLInjector
            ${PROJECT_SOURCES}
        )
endif()

add_manifest(SimpleDLLInjector)

target_link_libraries(SimpleDLLInjector PRIVATE InjectorFrontend)

set_target_properties(SimpleDLLInjector PROPERTIES
    ${BUNDLE_ID_OPTION}
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS SimpleDLLInjector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SimpleDLLInjector)
endif()

set(QT_DEPLOY_MODE debug)
if(CMAKE_BUILD_TYPE STREQUAL Release)
    set(QT_DEPLOY_MODE release)
endif()
add_custom_command(TARGET SimpleDLLInjector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${FILE_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SimpleDLLInjector> ${FILE_OUTPUT_DIR}/
        COMMAND ${Qt6_DIR}/../../../bin/windeployqt.exe --${QT_DEPLOY_MODE} --no-quick-import --no-translations --no-system-d3d-compiler --no-opengl-sw --dir ${FILE_OUTPUT_DIR} $<TARGET_FILE:SimpleDLLInjector>
        COMMENT "Copying SimpleDLLInjector to ${FILE_OUTPUT_DIR}"
    )
add_subdirectory(frontend)
add_subdirectory(backend)

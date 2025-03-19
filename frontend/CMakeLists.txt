set(TARGET_NAME InjectorFrontend)

set(SOURCES
        mainwindow.cpp
        processselect.cpp
    )

set(HEADERS
        mainwindow.h
        processselect.h
        qprocessinfolistmodel.h
    )

set(UI_FILES
        mainwindow.ui
        processselect.ui
          )

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS} ${UI_FILES})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.)
target_link_libraries(${TARGET_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core InjectorBackend)

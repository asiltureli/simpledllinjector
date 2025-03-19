add_subdirectory(processmanager)

set(TARGET_NAME InjectorBackend)

set(SOURCES
        backend.cpp
)

set(HEADERS
        backend.h
)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS}
    )

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.)

target_link_libraries(${TARGET_NAME} PUBLIC ProcessManagerx64)

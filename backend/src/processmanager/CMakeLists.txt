set(TARGET_NAME86 ProcessManagerx86)
set(TARGET_NAME64 ProcessManagerx64)

set(SOURCES
        processmanager.cpp
)

set(HEADERS
        processmanager.h
        processmanagerx64.h
        processmanagerx86.h
        processinfo.h
)


# x86
if(COMPILING_X86)

        add_library(${TARGET_NAME86} STATIC ${SOURCES} ${HEADERS})

        set_target_properties(${TARGET_NAME86} PROPERTIES
                        COMPILE_OPTIONS "/arch:IA32"
                        LINK_OPTIONS "/MACHINE:X86"
                )

target_include_directories(${TARGET_NAME86} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.)

# x64
else()
        add_library(${TARGET_NAME64} STATIC ${SOURCES} ${HEADERS})

        target_include_directories(${TARGET_NAME64} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.)

endif()

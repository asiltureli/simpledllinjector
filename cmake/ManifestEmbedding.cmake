function(add_manifest target_name)
    set(MANIFEST_FILE "${MY_CMAKE_MODULE_PATH}/../resources/app.manifest")

    if(NOT EXISTS "${MANIFEST_FILE}")
        message(FATAL_ERROR "Manifest file ${MANIFEST_FILE} does not exist.")
    endif()

        target_sources(${target_name} PRIVATE ${MANIFEST_FILE})
		
    target_link_options(${target_name} PRIVATE "/MANIFEST:NO")

    add_custom_command(
        TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_MT} -manifest "${MANIFEST_FILE}" -outputresource:$<TARGET_FILE:${target_name}>;#1
        COMMENT "Embedding custom manifest into ${target_name}"
    )
endfunction()

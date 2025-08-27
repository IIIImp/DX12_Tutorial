cmake_minimum_required(VERSION 3.15)
project(DX12_Tutorial)

set(CMAKE_CXX_STANDARD 17)

set(SUBSYSTEM_TYPE WINDOWS)

set(BASELIB
    kernel32.lib user32.lib gdi32.lib winspool.lib 
    shell32.lib ole32.lib oleaut32.lib uuid.lib 
    comdlg32.lib advapi32.lib
)

set(DEBUGLIB
    ucrtd.lib
    vcruntimed.lib
    msvcrtd.lib
    legacy_stdio_definitions.lib
)

set(DXLIB
    d3d12.lib
    dxgi.lib
    dxguid.lib
    d3dcompiler.lib
    windowscodecs.lib
)

set(DEMO_NAME
    001-InitWindow
    002-BetterWindow
    003-Rect
    004-Texture
    005-Cube
    006-FP
)

foreach(CURRENT_NAME ${DEMO_NAME})
    # Add all compilable resource
    file(GLOB ${CURRENT_NAME}_SOURCES
        "${CURRENT_NAME}/*.cpp"
        "${CURRENT_NAME}/*.h"
        "${CURRENT_NAME}/*.hpp"
    )
    
    add_executable(${CURRENT_NAME} ${${CURRENT_NAME}_SOURCES})
    
    add_definitions(-DUNICODE -D_UNICODE)
    
    target_include_directories(${CURRENT_NAME} PRIVATE ${CURRENT_NAME})
    
    set_target_properties(${CURRENT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:${SUBSYSTEM_TYPE} /ENTRY:WinMain"
        )
    
    target_link_libraries(${CURRENT_NAME} PRIVATE
    ${BASELIB}
    ${DEBUGLIB}
    ${DXLIB}
        )
        
    set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_NAME}/Resources")
    if(EXISTS ${RESOURCE_DIR})
        add_custom_command(TARGET ${CURRENT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${RESOURCE_DIR}"
            "$<TARGET_FILE_DIR:${CURRENT_NAME}>"
            COMMENT "Copying resources for ${CURRENT_NAME}"
        )
        add_custom_command(TARGET ${CURRENT_NAME} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${RESOURCE_DIR}"
              "${CMAKE_BINARY_DIR}/"
              COMMENT "Copying resources for ${CURRENT_NAME}"
        )
    endif()
endforeach()
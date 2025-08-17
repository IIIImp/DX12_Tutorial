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
)

set(DEMO_NAME
    001-InitWindow
    002-BetterWindow
    003-Rect
)

foreach(CURRENT_NAME ${DEMO_NAME})
    add_executable(${CURRENT_NAME} ${CURRENT_NAME}.cpp)
    add_definitions(-DUNICODE -D_UNICODE)
    
    set_target_properties(${CURRENT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:${SUBSYSTEM_TYPE} /ENTRY:WinMain"
        )
    
    target_link_libraries(${CURRENT_NAME} PRIVATE
    ${BASELIB}
    ${DEBUGLIB}
    ${DXLIB}
        )
endforeach()
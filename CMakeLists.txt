cmake_minimum_required(VERSION 3.15)
project(DX12_Tutorial)

set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} 001-InitWindow.cpp)
set(SUBSYSTEM_TYPE WINDOWS)
    
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:${SUBSYSTEM_TYPE} /ENTRY:WinMain"
    )
    
target_link_libraries(${PROJECT_NAME} PRIVATE
    kernel32.lib user32.lib gdi32.lib winspool.lib 
    shell32.lib ole32.lib oleaut32.lib uuid.lib 
    comdlg32.lib advapi32.lib
    )
    
target_link_libraries(${PROJECT_NAME} PRIVATE
    ucrtd.lib
    vcruntimed.lib
    msvcrtd.lib
    legacy_stdio_definitions.lib
    )
    
add_definitions(-DUNICODE -D_UNICODE)

target_link_libraries(${PROJECT_NAME} PRIVATE
    d3d12.lib
    dxgi.lib
    dxguid.lib
)
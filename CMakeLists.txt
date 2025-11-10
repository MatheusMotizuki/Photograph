cmake_minimum_required(VERSION 3.20)
project(photoGraph)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Force static runtime library (Windows only)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

include(FetchContent)

# SDL2 (cross-platform via FetchContent)
set(BUILD_SHARED_LIBS OFF)
set(SDL_STATIC ON)
set(SDL_AUDIO OFF CACHE BOOL "" FORCE)
set(SDL_DISABLE_AUDIO ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL2)

# ImGui (cross-platform via FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9b
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

# ImNodes (cross-platform via FetchContent)
FetchContent_Declare(
    imnodes
    GIT_REPOSITORY https://github.com/Nelarius/imnodes.git
    GIT_TAG v0.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imnodes)

# Use vcpkg for OpenCV, libhv, cpr (cross-platform)
find_package(OpenCV REQUIRED)
find_package(libhv CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)

# ImGui source files
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

set(IMNODES_SOURCES
    ${imnodes_SOURCE_DIR}/imnodes.cpp
)

# Executable (WIN32 flag only on Windows)
if(WIN32)
    add_executable(photoGraph WIN32
        source/Application.cpp
        source/GUI.cpp
        source/main.cpp
        source/style.cpp
        source/node/NodeBase.cpp
        source/node/NodeMenu.cpp
        source/node/submodules/io/Input.cpp
        source/node/submodules/io/Preview.cpp
        source/node/submodules/io/Download.cpp
        source/node/submodules/Blur.cpp
        source/node/submodules/Brightness.cpp
        source/node/submodules/Monochrome.cpp
        source/node/submodules/RGB.cpp
        ${IMGUI_SOURCES}
        ${IMNODES_SOURCES}
    )
else()
    add_executable(photoGraph
        source/Application.cpp
        source/GUI.cpp
        source/main.cpp
        source/style.cpp
        source/node/NodeBase.cpp
        source/node/NodeMenu.cpp
        source/node/submodules/io/Input.cpp
        source/node/submodules/io/Preview.cpp
        source/node/submodules/io/Download.cpp
        source/node/submodules/Blur.cpp
        source/node/submodules/Brightness.cpp
        source/node/submodules/Monochrome.cpp
        source/node/submodules/RGB.cpp
        ${IMGUI_SOURCES}
        ${IMNODES_SOURCES}
    )
endif()

# Include directories
target_include_directories(photoGraph PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${imnodes_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# Fix ImNodes math operators
target_compile_definitions(photoGraph PRIVATE IMGUI_DEFINE_MATH_OPERATORS)

# Link libraries
target_link_libraries(photoGraph PRIVATE 
    SDL2::SDL2-static
    SDL2::SDL2main
    ${OpenCV_LIBS}
    hv_static
    cpr::cpr
)

# Precompiled headers
target_precompile_headers(photoGraph PRIVATE
    <SDL.h>
    <imgui.h>
    <imnodes.h>
)
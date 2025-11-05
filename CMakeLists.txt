cmake_minimum_required(VERSION 3.20)
project(photoGraph)

# Include FetchContent for downloading dependencies
include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
set(SDL_STATIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL2)

# Disable SDL audio subsystem
set(SDL_AUDIO OFF CACHE BOOL "" FORCE)
set(SDL_DISABLE_AUDIO ON CACHE BOOL "" FORCE)

# Fetch ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9b
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

# Fetch ImNodes
FetchContent_Declare(
    imnodes
    GIT_REPOSITORY https://github.com/Nelarius/imnodes.git
    GIT_TAG v0.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imnodes)
if(NOT imnodes_POPULATED)
    FetchContent_Populate(imnodes)
endif()

FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG v11.4.6
)
FetchContent_MakeAvailable(ixwebsocket)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(cpr)

# Find OpenCV (system installation)
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)

# Configure ImGui source files
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)
# Set ImNodes source files
set(IMNODES_SOURCES
    ${imnodes_SOURCE_DIR}/imnodes.cpp
)

add_executable(photoGraph 
    source/Application.cpp
    source/GUI.cpp
    source/main.cpp
    source/style.cpp
    # --
    source/node/NodeBase.cpp
    source/node/NodeMenu.cpp
    # --
    source/node/submodules/io/Input.cpp
    source/node/submodules/io/Preview.cpp
    source/node/submodules/io/Download.cpp
    # --
    source/node/submodules/Blur.cpp
    source/node/submodules/Brightness.cpp
    source/node/submodules/Monochrome.cpp
    source/node/submodules/RGB.cpp
    # --
    ${IMGUI_SOURCES}
    ${IMNODES_SOURCES}
)

# Configure include directories
target_include_directories(photoGraph PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${imnodes_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

if (WIN32)
    # Windows-specific configuration
    find_package(ZLIB REQUIRED)
    target_link_libraries(photoGraph PRIVATE gdiplus ZLIB::ZLIB)
else()
    # Linux needs curlpp include paths
    target_include_directories(photoGraph PRIVATE
        ${curlpp_SOURCE_DIR}/src
        ${CURL_INCLUDE_DIRS}
    )
endif()

# Define IMGUI_DEFINE_MATH_OPERATORS to fix ImNodes build issue
target_compile_definitions(photoGraph PRIVATE IMGUI_DEFINE_MATH_OPERATORS)

target_link_libraries(photoGraph PRIVATE 
    SDL2::SDL2-static
    SDL2::SDL2main
    ixwebsocket
    cpr::cpr
    ${OpenCV_LIBS}
)

target_precompile_headers(photoGraph PRIVATE
    <SDL.h>
    <imgui.h>
    <imnodes.h>
)
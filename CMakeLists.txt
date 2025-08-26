cmake_minimum_required(VERSION 3.20)
project(photoGraph)

# Include FetchContent for downloading dependencies
include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
set(SDL_STATIC ON)

# Fetch SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL2)

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
FetchContent_MakeAvailable(imnodes) # Explicitly populate imnodes to handle it specially
if(NOT imnodes_POPULATED)
    FetchContent_Populate(imnodes)
    # We don't call add_subdirectory for imnodes because it doesn't have a CMakeLists.txt file
endif()

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

set(BUILD_opencv_apps OFF CACHE BOOL "")
set(BUILD_opencv_calib3d OFF CACHE BOOL "")
set(BUILD_opencv_dnn OFF CACHE BOOL "")
set(BUILD_opencv_features2d OFF CACHE BOOL "")
set(BUILD_opencv_flann OFF CACHE BOOL "")
set(BUILD_opencv_gapi OFF CACHE BOOL "")
set(BUILD_opencv_ml OFF CACHE BOOL "")
set(BUILD_opencv_photo OFF CACHE BOOL "")
set(BUILD_opencv_stitching OFF CACHE BOOL "")
set(BUILD_opencv_video OFF CACHE BOOL "")
set(BUILD_opencv_videoio OFF CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")
set(BUILD_PERF_TESTS OFF CACHE BOOL "")
set(BUILD_EXAMPLES OFF CACHE BOOL "")
set(WITH_TESTS OFF CACHE BOOL "")

include(FetchContent)
FetchContent_Declare(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv.git
    GIT_TAG 4.10.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(opencv)

add_executable(photoGraph 
    src/main.cpp
    src/Application.cpp
    src/GUI.cpp
    src/node/NodeBase.cpp
    src/node/submodules/io/Input.cpp
    ${IMGUI_SOURCES}
    ${IMNODES_SOURCES}
)

# Configure include directories
target_include_directories(photoGraph PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${imnodes_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENCV_CONFIG_FILE_INCLUDE_DIR}
    ${OPENCV_MODULE_opencv_core_LOCATION}/include
    ${OPENCV_MODULE_opencv_highgui_LOCATION}/include
)

if (WIN32)
    target_link_libraries(photoGraph PRIVATE gdiplus)
endif()

# Define IMGUI_DEFINE_MATH_OPERATORS to fix ImNodes build issue
target_compile_definitions(photoGraph PRIVATE IMGUI_DEFINE_MATH_OPERATORS)

# Link against SDL2
target_link_libraries(photoGraph PRIVATE 
    SDL2::SDL2-static 
    SDL2::SDL2main 
    opencv_core 
    opencv_highgui
)

target_precompile_headers(photoGraph PRIVATE
    <SDL.h>
    <imgui.h>
    <imnodes.h>
)
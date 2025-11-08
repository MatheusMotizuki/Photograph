# this cmake can be a little overkill 
# compile EVERYTHING statically, but
# hey, no more dependency headaches.
cmake_minimum_required(VERSION 3.20)
project(photoGraph)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Force static runtime library for all dependencies
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include(FetchContent)

# SDL2
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

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9b
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

# ImNodes
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

# libhv
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(HV_SHARED OFF CACHE BOOL "" FORCE)
set(HV_STATIC ON CACHE BOOL "" FORCE)
set(HV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(HV_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(HV_BUILD_BENCHMARK OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    libhv
    GIT_REPOSITORY https://github.com/ithewei/libhv.git
    GIT_TAG v1.3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(libhv)

# CPR (with curl built-in, no zlib)
set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE) # use built-in curl
set(CURL_ZLIB OFF CACHE BOOL "" FORCE)           # disable zlib in curl
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_USE_OPENSSL ON CACHE BOOL "" FORCE)

set(BUILD_CURL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_CURL_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_CURL_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(cpr)

# OpenCV
set(BUILD_LIST "core,imgproc,imgcodecs" CACHE STRING "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_opencv_apps OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_PERF_TESTS OFF CACHE BOOL "" FORCE)
set(WITH_JPEG ON CACHE BOOL "" FORCE)
set(WITH_PNG ON CACHE BOOL "" FORCE)
set(WITH_TIFF OFF CACHE BOOL "" FORCE)
set(WITH_WEBP OFF CACHE BOOL "" FORCE)
set(BUILD_JPEG ON CACHE BOOL "" FORCE)
set(BUILD_PNG ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv.git
    GIT_TAG 4.10.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(opencv)

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

# Executable
if(WIN32)
    add_executable(photoGraph WIN32
        source/Application.cpp
        source/GUI.cpp
        source/main.cpp
        source/style.cpp
        # -- Node sources
        source/node/NodeBase.cpp
        source/node/NodeMenu.cpp
        # -- Node submodules
        source/node/submodules/io/Input.cpp
        source/node/submodules/io/Preview.cpp
        source/node/submodules/io/Download.cpp
        source/node/submodules/Blur.cpp
        source/node/submodules/Brightness.cpp
        source/node/submodules/Monochrome.cpp
        source/node/submodules/RGB.cpp
        # -- GUI sources
        ${IMGUI_SOURCES}
        ${IMNODES_SOURCES}
    )
else()
    add_executable(photoGraph
        source/Application.cpp
        source/GUI.cpp
        source/main.cpp
        source/style.cpp
        # -- Node sources
        source/node/NodeBase.cpp
        source/node/NodeMenu.cpp
        # -- Node submodules
        source/node/submodules/io/Input.cpp
        source/node/submodules/io/Preview.cpp
        source/node/submodules/io/Download.cpp
        source/node/submodules/Blur.cpp
        source/node/submodules/Brightness.cpp
        source/node/submodules/Monochrome.cpp
        source/node/submodules/RGB.cpp
        # -- GUI sources
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
    ${opencv_SOURCE_DIR}/modules/core/include
    ${opencv_SOURCE_DIR}/modules/imgproc/include
    ${opencv_SOURCE_DIR}/modules/imgcodecs/include
    ${CMAKE_BINARY_DIR}
)

# Fix ImNodes math operators
target_compile_definitions(photoGraph PRIVATE IMGUI_DEFINE_MATH_OPERATORS)

# Link libraries
target_link_libraries(photoGraph PRIVATE 
    SDL2::SDL2-static
    SDL2::SDL2main
    hv
    cpr::cpr
    opencv_core
    opencv_imgproc
    opencv_imgcodecs
)

# Precompiled headers
target_precompile_headers(photoGraph PRIVATE
    <SDL.h>
    <imgui.h>
    <imnodes.h>
)

cmake_minimum_required(VERSION 3.22)
project(PhotoGraph C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# Include FetchContent for downloading dependencies
include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
set(SDL_STATIC ON)

set(CMAKE_CXX_STANDARD 17)

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
    GIT_REPOSITORY https://github.com/MatheusMotizuki/imnodes.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imnodes)
if(NOT imnodes_POPULATED)
    FetchContent_Populate(imnodes)
endif()

# Fetch SDL2 last, this takes a lot to build
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

# Configure ImGui source files (always use OpenGL3 backend)
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# Set ImNodes source files
set(IMNODES_SOURCES
    ${imnodes_SOURCE_DIR}/imnodes.cpp
)

add_executable(photoGraph 
    src/Application.cpp
    src/GUI.cpp
    src/main.cpp
    src/style.cpp
    # --
    src/node/NodeBase.cpp
    src/node/NodeMenu.cpp
    # --
    src/node/submodules/io/Input.cpp
    src/node/submodules/io/Output.cpp
    # --
    src/node/submodules/Blur.cpp
    src/node/submodules/Brightness.cpp
    src/node/submodules/Monochrome.cpp
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
)

# Define IMGUI_DEFINE_MATH_OPERATORS to fix ImNodes build issue
target_compile_definitions(photoGraph PRIVATE IMGUI_DEFINE_MATH_OPERATORS)

# Platform-specific configuration
if(EMSCRIPTEN)
    # WebAssembly specific settings
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    
    target_compile_definitions(photoGraph PRIVATE 
        IMGUI_DISABLE_FILE_FUNCTIONS=1
    )
    
    target_compile_options(photoGraph PRIVATE 
        "-sUSE_SDL=2"
    )
    
    target_link_options(photoGraph PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_WEBGL2=1"
        "-sFULL_ES3=1"
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sNO_EXIT_RUNTIME=0"
        "-sASSERTIONS=1"
        "-sMAX_WEBGL_VERSION=2"
        "-sMIN_WEBGL_VERSION=2"
        "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/shell_minimal.html"
    )
    
    set_target_properties(photoGraph PROPERTIES OUTPUT_NAME "index")
else()
    # Native build settings
    if (WIN32)
        target_link_libraries(photoGraph PRIVATE gdiplus)
    endif()
    
    # Link against SDL2
    target_link_libraries(photoGraph PRIVATE 
        SDL2::SDL2-static
        SDL2::SDL2main
    )

    target_precompile_headers(photoGraph PRIVATE
        <SDL.h>
        <imgui.h>
        <imnodes.h>
    )
endif()
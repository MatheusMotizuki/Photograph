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
    # PATCH_COMMAND powershell -Command "(Get-Content ${imnodes_SOURCE_DIR}/imnodes.cpp) -replace 'FILE\\* file = ImFileOpen\\(file_name, \"wt\"\\);', '#ifndef IMGUI_DISABLE_FILE_FUNCTIONS`nFILE* file = ImFileOpen(file_name, \"wt\");' -replace 'fwrite\\(data, sizeof\\(char\\), data_size, file\\);', 'fwrite(data, sizeof(char), data_size, file);`nfclose(file);`n#endif' | Set-Content ${imnodes_SOURCE_DIR}/imnodes.cpp"
)
FetchContent_MakeAvailable(imnodes) # Explicitly populate imnodes to handle it specially
if(NOT imnodes_POPULATED)
    FetchContent_Populate(imnodes)
    # We don't call add_subdirectory for imnodes because it doesn't have a CMakeLists.txt file
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

# Configure ImGui source files
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
)

# Add appropriate backend based on platform
if(EMSCRIPTEN)
    list(APPEND IMGUI_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
else()
    list(APPEND IMGUI_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp)
endif()

# Set ImNodes source files
set(IMNODES_SOURCES
    ${imnodes_SOURCE_DIR}/imnodes.cpp
)

add_executable(photoGraph 
    src/main.cpp
    src/Application.cpp
    src/GUI.cpp
    src/node/NodeBase.cpp
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

if (WIN32)
    target_link_libraries(photoGraph PRIVATE gdiplus)
endif()

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
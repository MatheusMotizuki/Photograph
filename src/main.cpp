#include "Application.hpp"
#include <iostream>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

int main(int argc, char* argv[])
{
    std::cout << "Starting PhotoGraph application..." << std::endl;
    
#ifdef EMSCRIPTEN
    std::cout << "Running in WebAssembly mode" << std::endl;
#else
    std::cout << "Running in native mode" << std::endl;
#endif

    Application app("PhotoGraph", 1280, 720);
    
    std::cout << "Initializing application..." << std::endl;
    if (!app.initialize())
    {
        std::cerr << "Failed to initialize application" << std::endl;
        return 1;
    }
    
    std::cout << "Application initialized successfully, starting main loop..." << std::endl;
    app.run();
    
    std::cout << "Application shutting down..." << std::endl;
    return 0;
}
#include "Application.hpp"
#include <iostream>

#include <emscripten.h>

int main(int argc, char* argv[])
{
    std::cout << "Starting PhotoGraph application..." << std::endl;

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

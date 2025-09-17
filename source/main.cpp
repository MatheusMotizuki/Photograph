#include "Application.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    // Create application instance
    Application app("photoGraph", 1280, 720);

    // Initialize the application
    if (!app.initialize())
    {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }

    // Run the application
    app.run();

    return 0;
}
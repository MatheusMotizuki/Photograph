#ifdef _WIN32
#include <winsock2.h>
#endif

#include "Application.hpp"
#include <iostream>

int main(int argc, char** argv)
{

#ifdef _WIN32
    WSADATA wsaData;
    int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaerr != 0) {
        std::cerr << "WSAStartup failed: " << wsaerr << std::endl;
        return 1;
    }
#endif

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

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
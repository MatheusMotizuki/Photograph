#pragma once

#include <string>
#include <memory>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "GUI.hpp"

class Application {
private:
    std::string m_title;
    int m_width;
    int m_height;
    bool m_running;
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;
    std::unique_ptr<GUI> m_gui;

public:
    Application(const std::string& title, int width, int height);
    ~Application();

    bool initialize();
    void run();
    bool processEvents();
    void update();
    void render();
    void shutdown();
    
    // For Emscripten main loop
    static void emscripten_loop(void* arg);
};
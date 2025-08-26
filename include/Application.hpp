#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include "GUI.hpp"

class Application {
public:
    Application(const std::string& title, int width, int height);
    ~Application();

    bool initialize();
    void run();
    void shutdown();

private:
    bool processEvents();
    void update();
    void render();

    std::string m_title;
    int m_width;
    int m_height;
    bool m_running;

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    std::unique_ptr<GUI> m_gui;
};
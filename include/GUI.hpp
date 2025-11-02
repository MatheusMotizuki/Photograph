#pragma once

#include <SDL2/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"
#include <string>
#include <vector>
#include <memory>

#include "node/NodeBase.hpp"
#include "node/NodeMenu.hpp"

class GUI {
public:
    GUI(SDL_Window* window, SDL_GLContext gl_context);
    ~GUI();

    bool initialize();
    void shutdown();
    void processEvent(const SDL_Event& event);
    void render();

    void setStyle();
    void popStyle();

    // Node factory
    std::unique_ptr<NodeBase> createNode(NodeMenu::NodeType type);

private:
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;
    bool m_initialized;

    std::vector<std::unique_ptr<NodeBase>> n_nodes;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar 
        | ImGuiWindowFlags_NoCollapse 
        | ImGuiWindowFlags_MenuBar;
};
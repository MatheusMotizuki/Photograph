#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <memory>
#include "imnodes.h"

#include "node/NodeBase.hpp"
// #include "Links.h"
#include "node/NodeMenu.hpp"

class GUI {
public:
    GUI(SDL_Window* window, SDL_Renderer* renderer);
    ~GUI();

    bool initialize();
    void shutdown();
    
    void processEvent(const SDL_Event& event);
    void newFrame();
    void render();

    void setStyle();
    void popStyle();

    // Node factory
    std::unique_ptr<NodeBase> createNode(NodeMenu::NodeType type);
        
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_initialized;

    std::vector<std::unique_ptr<NodeBase>> n_nodes;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar 
    | ImGuiWindowFlags_NoCollapse 
    | ImGuiWindowFlags_MenuBar;    
};
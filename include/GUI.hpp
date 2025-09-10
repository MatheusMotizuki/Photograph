#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imnodes.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#include <SDL2/SDL.h>

#include "node/NodeBase.hpp"
#include "node/NodeMenu.hpp"

// In 'n' out
#include "node/submodules/io/Input.hpp"
#include "node/submodules/io/Output.hpp"
// Common
#include "node/submodules/Monochrome.hpp"
#include "node/submodules/Brightness.hpp"
#include "node/submodules/Blur.hpp"

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

    inline void certainDeathNode(
        std::vector<std::unique_ptr<NodeBase>>& n_nodes,
        const std::unordered_set<int>& death_node
    );

    inline void certainDeathLink(
        std::vector<Link>& n_links, 
        std::unordered_set<int>& death_link
    );

    // Node factory
    std::unique_ptr<NodeBase> createNode(NodeMenu::NodeType type);

    std::unordered_set<int> death_node;
    std::unordered_set<int> death_link;

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_initialized;

    std::vector<std::unique_ptr<NodeBase>> n_nodes;
    std::vector<Link> n_links;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar 
    | ImGuiWindowFlags_NoCollapse
    | ImGuiWindowFlags_MenuBar;
};
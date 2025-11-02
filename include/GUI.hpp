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

// In 'n' out
#include "node/submodules/io/Input.hpp"
#include "node/submodules/io/Output.hpp"
// Common
#include "node/submodules/Monochrome.hpp"
#include "node/submodules/Brightness.hpp"
#include "node/submodules/Blur.hpp"

class GUI {
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_initialized;

    // nodes related
    std::vector<std::unique_ptr<NodeBase>> n_nodes;
    std::unordered_set<int> selected_nodes;

    // links related
    std::vector<Link> n_links;
    std::unordered_set<int> selected_links;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar 
    | ImGuiWindowFlags_NoCollapse
    | ImGuiWindowFlags_MenuBar;

private:

public:
    GUI(SDL_Window* window, SDL_GLContext gl_context);
    ~GUI();

    bool initialize();
    void shutdown();
    void processEvent(const SDL_Event& event);
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

private:
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;
    bool m_initialized;

    std::vector<std::unique_ptr<NodeBase>> n_nodes;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar 
        | ImGuiWindowFlags_NoCollapse 
        | ImGuiWindowFlags_MenuBar;
};
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

#include <SDL2/SDL.h>

#include "node/NodeMenu.hpp"

#include "stb_image.h"

#include "WebSocketClient.hpp"

class GUI {
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_initialized;

    // cursor related
    std::unordered_map<std::string, ImVec2> remoteMousePositions;
    SDL_Texture* cursor_texture = nullptr;
    int cursor_width = 0;
    int cursor_height = 0;

    // websocket
    WebSocketClient* wsClient = nullptr;

    // nodes related
    std::vector<std::unique_ptr<NodeBase>> n_nodes;
    std::unordered_set<int> selected_nodes;

    // links related
    std::vector<Link> n_links;
    std::unordered_set<int> selected_links;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar 
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_MenuBar;

    void initialOption();
    std::string generate_unique_code();

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

    std::unordered_set<int> death_node;
    std::unordered_set<int> death_link;

    std::string unique_code;

    int ConnectAndCreateSession(std::string route, std::string roomID);
    int ConnectAndJoinSession(std::string route, std::string roomID);
    void InitializeCallbacks();
};
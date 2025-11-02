#include "GUI.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GUI::GUI(SDL_Window* window, SDL_GLContext gl_context)
    : m_window(window)
    , m_gl_context(gl_context)
    , m_initialized(false)
{
}

GUI::~GUI()
{
    shutdown();
}

bool GUI::initialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr;
    // Use default font for web build
    io.Fonts->AddFontDefault();
#endif

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_window, m_gl_context);
    
#ifdef __EMSCRIPTEN__
    ImGui_ImplOpenGL3_Init("#version 300 es");
#else
    ImGui_ImplOpenGL3_Init("#version 330");
#endif

#ifndef __EMSCRIPTEN__
    SDL_SetWindowMinimumSize(SDL_GL_GetCurrentWindow(), 640, 480);
    SDL_SetWindowSize(SDL_GL_GetCurrentWindow(), 1280, 720);
    SDL_SetWindowPosition(SDL_GL_GetCurrentWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowResizable(SDL_GL_GetCurrentWindow(), SDL_TRUE);
    SDL_SetWindowMaximumSize(SDL_GL_GetCurrentWindow(), 1920, 1080);
#endif

    n_nodes.push_back(std::make_unique<InputNode>());
    n_nodes.push_back(std::make_unique<DownloadNode>());

    m_initialized = true;
    return true;
}

void GUI::shutdown()
{
    if (m_initialized)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImNodes::DestroyContext();
        ImGui::DestroyContext();
        m_initialized = false;
    }
}

void GUI::processEvent(const SDL_Event& event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void GUI::setStyle()
{
    ImNodes::PushColorStyle(ImNodesCol_GridBackground, IM_COL32(18, 18, 18, 255));
    ImNodes::PushColorStyle(ImNodesCol_GridLine, IM_COL32(50, 50, 50, 95));

    ImNodes::PushColorStyle(ImNodesCol_BoxSelector, IM_COL32(138, 43, 226, 35));
    ImNodes::PushColorStyle(ImNodesCol_BoxSelectorOutline, IM_COL32(90, 20, 140, 180));
}

void GUI::popStyle()
{
    ImNodes::PopColorStyle(); // GridLine
    ImNodes::PopColorStyle(); // GridBackground

    ImNodes::PopColorStyle(); // BoxSelector
    ImNodes::PopColorStyle(); // BoxSelectorOutline
}

void GUI::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Dockspace setup
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::Begin("photoGraph", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_AlwaysAutoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoNav | 
        ImGuiWindowFlags_NoDecoration
    );
    GUI::setStyle();
    #ifdef __EMSCRIPTEN__
        // Use default font for web build
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    #else
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[18]); // Setup default font
    #endif
    ImNodes::BeginNodeEditor();

    const bool open_menu = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right, false) && selected_nodes.empty();

    if (open_menu) ImGui::OpenPopup("add node");

    if (m_nodeMenu.Draw()) {
        ImVec2 position = m_nodeMenu.GetClickPos();
        std::unique_ptr<NodeBase> node = m_nodeMenu.CreateNode(m_nodeMenu.GetNodeType());
        if (node) {
            ImNodes::SetNodeScreenSpacePos(node->GetId(), position);
            n_nodes.push_back(std::move(node));
        }
    }

    // clear both before checking
    selected_nodes.clear();
    selected_links.clear();

    for (const auto& node : n_nodes) {
        node->Draw();
        int nodeID = node->GetId();
        bool selected = ImNodes::IsNodeSelected(node->GetId());
        if (selected) {
            selected_nodes.insert(node->GetId());
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right, false)) {
                ImGui::OpenPopup((node->GetInternalTitle() + "_" + std::to_string(node->GetId())).c_str());
            }
        }
        node->Description();
    }

    for (const Link& link : n_links) {
        ImNodes::Link(link.id, link.init_attr, link.end_attr);

        if (ImNodes::IsLinkSelected(link.id)) {
            selected_links.insert(link.id);
        }
    }

    // delete selected nodes and links
    if (ImGui::IsKeyPressed(ImGuiKey_Delete, false)) {
        for (int id : selected_nodes) {
            death_node.insert(id);
        }

        for (int id : selected_links) {
            death_link.insert(id);
        }
    }

    if (!death_node.empty() && 
        ImGui::IsKeyPressed(ImGuiKey_Delete, false)
    ) { certainDeathNode(n_nodes, death_node); death_node.clear(); }

    // TODO: check if the node attached to this link
    // was delete, if so delete this link also.
    if (!death_link.empty() && 
        ImGui::IsKeyPressed(ImGuiKey_Delete, false)
    ) { certainDeathLink(n_links, death_link); death_link.clear(); }

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    ImNodes::EndNodeEditor();

    int start_attr, end_attr;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr)) {
        Link link;
        link.id = Link::link_next_id++;
        link.init_attr = start_attr;
        link.end_attr = end_attr;
        n_links.push_back(link);
    }

    int link_id;
    if (ImNodes::IsLinkDestroyed(&link_id)) {
        n_links.erase(
            std::remove_if(n_links.begin(), n_links.end(),
            [link_id](const Link& link) { return link.id == link_id; }),
            n_links.end()
        );
    }

    ImGui::PopFont();
    GUI::popStyle();
    ImGui::End();
}

void GUI::render()
{
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

inline void GUI::certainDeathNode(std::vector<std::unique_ptr<NodeBase>>& n_nodes, const std::unordered_set<int>& death_node) {
    n_nodes.erase(
    std::remove_if(n_nodes.begin(), n_nodes.end(),
        [&death_node](const std::unique_ptr<NodeBase>& node){
            if (!node->IsProtected()) return false; // do not remove if protected
            return death_node.count(node->GetId()) > 0;
        }),
    n_nodes.end());
}

inline void GUI::certainDeathLink(std::vector<Link>& n_links, std::unordered_set<int>& death_link){
    n_links.erase(
        std::remove_if(n_links.begin(), n_links.end(),
        [&death_link](const Link& link) {
            return death_link.count(link.id) > 0;
        }),
    n_links.end());
}
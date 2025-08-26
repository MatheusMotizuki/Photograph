#include "GUI.hpp"
#include <algorithm>
#include <iostream>

#include <vector>
#include <memory>

#include <unordered_set>

#include "node/submodules/Monochrome.hpp"

GUI::GUI(SDL_Window* window, SDL_Renderer* renderer)
    : m_window(window)
    , m_renderer(renderer)
    , m_initialized(false)
{
}

GUI::~GUI()
{
    shutdown();
}

bool GUI::initialize()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = nullptr; // Disable .ini file loading

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    if (!ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer))
    {
        return false;
    }
    
    if (!ImGui_ImplSDLRenderer2_Init(m_renderer))
    {
        ImGui_ImplSDL2_Shutdown();
        ImNodes::DestroyContext();
        ImGui::DestroyContext();
        return false;
    }

    SDL_SetWindowMinimumSize(SDL_GL_GetCurrentWindow(), 640, 480);
    SDL_SetWindowSize(SDL_GL_GetCurrentWindow(), 1280, 720);
    SDL_SetWindowPosition(SDL_GL_GetCurrentWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowResizable(SDL_GL_GetCurrentWindow(), SDL_TRUE);
    SDL_SetWindowMaximumSize(SDL_GL_GetCurrentWindow(), 1920, 1080);

    m_initialized = true;
    return true;
}

void GUI::shutdown()
{
    if (m_initialized)
    {
        ImGui_ImplSDLRenderer2_Shutdown();
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
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();    

    // Dockspace setup
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::Begin("photoGraph", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration);
    GUI::setStyle();
    ImNodes::BeginNodeEditor();
    
    NodeMenu Menu;
    if (Menu.Draw()) {
        ImVec2 position = Menu.GetClickPos();

        std::unique_ptr<NodeBase> node = createNode(Menu.GetNodeType());
        if (node) {
            ImNodes::SetNodeScreenSpacePos(node->GetId(), position);
            n_nodes.push_back(std::move(node));
        }
    }

    for (const auto& node : n_nodes) {
        node->Draw();
    }

    std::unordered_set<int> death_marks;
    for (const auto& node : n_nodes) {
        if (node->IsSelected()) {
            death_marks.insert(node->GetId());
        }
    }

    if (!death_marks.empty() && ImGui::IsKeyPressed(ImGuiKey_Delete, false)) {
        std::cout << "Delete key pressed. Attempting to remove selected nodes..." << std::endl;
        size_t before = n_nodes.size();
        n_nodes.erase(
            std::remove_if(n_nodes.begin(), n_nodes.end(),
                [&death_marks](const std::unique_ptr<NodeBase>& node){
                    if (!node->IsProtected()) return false;
                    if (death_marks.count(node->GetId()) > 0) {
                        std::cout << "Removing node with ID: " << node->GetId() << std::endl;
                        return true;
                    }
                    return false;
                }),
            n_nodes.end());
        size_t after = n_nodes.size();
        std::cout << "Removed " << (before - after) << " nodes." << std::endl;
    }
    
    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    ImNodes::EndNodeEditor();
    GUI::popStyle();
    ImGui::End();
}

void GUI::render()
{
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    SDL_RenderSetScale(m_renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);
}

std::unique_ptr<NodeBase> GUI::createNode(NodeMenu::NodeType type)
{
    switch (type)
    {
    case NodeMenu::NodeType::Monochrome:
        return std::make_unique<MonochromeNode>();
        break;
    default:
        return nullptr;
        break;
    }
}
#include "GUI.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_set>

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <SDL2/SDL_opengles2.h>
#endif

// In 'n' out
#include "node/submodules/io/Input.hpp"
#include "node/submodules/io/Output.hpp"
// Common
#include "node/submodules/Monochrome.hpp"
#include "node/submodules/Blur.hpp"

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
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    
#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr; // Disable .ini file for web
#else
    io.IniFilename = nullptr; // Disable .ini file
    
    // Load fonts only for native builds
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Black.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-BlackItalic.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Bold.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-BoldItalic.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-ExtraBold.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-ExtraBoldItalic.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-ExtraLight.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-ExtraLightItalic.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Italic.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Light.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-LightItalic.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Medium.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-MediumItalic.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Regular.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-SemiBold.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-SemiBoldItalic.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Thin.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-ThinItalic.ttf", 18.0f);
#endif

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_gl_context);
    ImGui_ImplOpenGL3_Init("#version 300 es");

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

void GUI::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Dockspace setup
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::Begin("photoGraph", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration);
    GUI::setStyle();
    
    // Use default font for WASM, Inter font for native
#ifndef __EMSCRIPTEN__
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[11]); // Setup default font
#endif
    
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

    // Create unique input and output nodes only once and draw them first
    static std::unique_ptr<InputNode> input = std::make_unique<InputNode>();
    static std::unique_ptr<OutputNode> output = std::make_unique<OutputNode>();
    
    // unfortunately we have to do this...
    static bool initialized = false;
    if (!initialized) {
        ImNodes::SetNodeScreenSpacePos(input->GetId(), ImVec2(60, 50));
        ImNodes::SetNodeScreenSpacePos(output->GetId(), ImVec2(900, 350));
        initialized = true;
    }
    input->Draw();
    output->Draw();

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
    
#ifndef __EMSCRIPTEN__
    ImGui::PopFont();
#endif
    
    GUI::popStyle();
    ImGui::End();

    ImGui::Render();
    
    int display_w, display_h;
    SDL_GetWindowSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::unique_ptr<NodeBase> GUI::createNode(NodeMenu::NodeType type)
{
    switch (type)
    {
    case NodeMenu::NodeType::MonochromeNode:
        return std::make_unique<MonochromeNode>();
        break;
    case NodeMenu::NodeType::BlurNode:
        return std::make_unique<BlurNode>();
        break;
    default:
        return nullptr;
        break;
    }
}
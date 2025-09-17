#include "GUI.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GUI::GUI(SDL_Window* window, SDL_Renderer* renderer)
    : m_window(window)
    , m_renderer(renderer)
    , m_initialized(false)
{
    std::cout << "GUI says: hello world!" << std::endl;
}

GUI::~GUI()
{
    std::cout << "GUI says: bye bye" << std::endl;
    n_links.clear();
    n_nodes.clear();
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

    // montserrat
    // io.Fonts->AddFontFromFileTTF("assets/fonts/montserrat/Montserrat-Black.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/montserrat/Montserrat-Bold.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/montserrat/Montserrat-SemiBold.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/montserrat/Montserrat-Medium.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/montserrat/Montserrat-Regular.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/montserrat/Montserrat-Italic.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/montserrat/Montserrat-Light.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/montserrat/Montserrat-Thin.ttf", 18.0f);

    // inter
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

    // consolas
    io.Fonts->AddFontFromFileTTF("assets/fonts/consolas/Inconsolata-Regular.ttf", 16.0f);

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

    ImGui::Begin("photoGraph", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_AlwaysAutoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoNav | 
        ImGuiWindowFlags_NoDecoration
    );
    GUI::setStyle();
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[18]); // Setup default font
    ImNodes::BeginNodeEditor();
    
    const bool open_menu = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        ImNodes::IsEditorHovered() && ImGui::IsMouseDown(1) && selected_nodes.empty();

    // Create unique input and output nodes only once and draw them first
    static std::unique_ptr<InputNode> input = std::make_unique<InputNode>(m_renderer); input->Draw();
    static std::unique_ptr<OutputNode> output = std::make_unique<OutputNode>(); output->Draw();

    NodeMenu Menu;
    if (open_menu) ImGui::OpenPopup("add node");
    if (Menu.Draw()) {
        ImVec2 position = Menu.GetClickPos();
        std::unique_ptr<NodeBase> node = createNode(Menu.GetNodeType());
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
        if (selected) selected_nodes.insert(node->GetId());
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

    if (!death_link.empty() && 
        ImGui::IsKeyPressed(ImGuiKey_Delete, false)
    ) { certainDeathLink(n_links, death_link); death_link.clear(); }

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    ImNodes::EndNodeEditor();

// TODO: fix this
    // for (auto& node : n_nodes) {
    //     int nodeID = node->GetId();
    //     bool hovered = ImNodes::IsNodeHovered(&nodeID);
    //     if (hovered) {
    //         if(ImGui::IsMouseDown(1)) ImGui::OpenPopup((node->GetInternalTitle() + "_" + std::to_string(node->GetId())).c_str());
    //     }
    //     node->Description();
    // }

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
    SDL_RenderSetScale(m_renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);
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

std::unique_ptr<NodeBase> GUI::createNode(NodeMenu::NodeType type)
{
    switch (type)
    {
    case NodeMenu::NodeType::MonochromeNode:
        return std::make_unique<MonochromeNode>();
        break;
    case NodeMenu::NodeType::BrightnessNode:
        return std::make_unique<BrightnessNode>();
        break;
    case NodeMenu::NodeType::BlurNode:
        return std::make_unique<BlurNode>();
        break;
    default:
        return nullptr;
        break;
    }
}
#include "GUI.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <random>
#include <string>

// function prototypes
std::string generate_unique_code();

GUI::GUI(SDL_Window* window, SDL_Renderer* renderer)
    : m_window(window)
    , m_renderer(renderer)
    , m_initialized(false)
{}

GUI::~GUI()
{
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
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Black.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-BlackItalic.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Bold.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-BoldItalic.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-ExtraBold.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-ExtraBoldItalic.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-ExtraLight.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-ExtraLightItalic.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Italic.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Light.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-LightItalic.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Medium.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-MediumItalic.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Regular.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-SemiBold.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-SemiBoldItalic.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-Thin.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter_18pt-ThinItalic.ttf", 18.0f);

    // consolas
    // io.Fonts->AddFontFromFileTTF("assets/fonts/consolas/Inconsolata-Regular.ttf", 16.0f);

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

    // Add the input and output nodes upon GUI
    // creation, so they'll be added only once
    n_nodes.push_back(std::make_unique<InputNode>(m_renderer));
    n_nodes.push_back(std::make_unique<DownloadNode>(m_renderer));

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

void initialOption()
{
    static bool first_frame = true;
    static int session_state = 0; // 0 = menu, 1 = show code, 2 = joining
    static std::string gen_code = "";
    const char* welcome_message = "Welcome to photoGraph";

    if (first_frame)
    {
        ImGui::OpenPopup(welcome_message);
        first_frame = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 12));

    if (ImGui::BeginPopupModal(welcome_message, NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
    {
        // State 0: Initial Menu
        if (session_state == 0)
        {
            ImGui::Text("Choose an option:");
            ImGui::Spacing();

            if (ImGui::Button("New Project", ImVec2(250, 40)))
            {
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("New Session", ImVec2(250, 40)))
            {
                gen_code = generate_unique_code();
                session_state = 1;
            }

            ImGui::Separator();
            ImGui::Spacing();

            static char session_code[64] = "";
            ImGui::SetNextItemWidth(250);
            ImGui::InputTextWithHint("##session_code", "Enter Session Code", session_code, IM_ARRAYSIZE(session_code));

            bool has_text = session_code[0] != '\0';
            ImGui::BeginDisabled(!has_text);
            if (ImGui::Button("Join Session", ImVec2(250, 40)))
            {
                std::cout << "Joining session: " << session_code << std::endl;
                // TODO: Start WebSocket client here
                // JoinWebSocketServer(session_code);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndDisabled();
        }
        // State 1: Show Generated Code
        else if (session_state == 1)
        {
            ImGui::Text("Your session code:");
            ImGui::Spacing();
            
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Use default font or monospace if available
            ImGui::SetNextItemWidth(250);
            ImGui::InputText("##gen_code", gen_code.data(), gen_code.size(), ImGuiInputTextFlags_ReadOnly);
            ImGui::PopFont();

            ImGui::Spacing();
            
            if (ImGui::Button("Copy to Clipboard", ImVec2(250, 40)))
            {
                ImGui::SetClipboardText(gen_code.c_str());
            }

            if (ImGui::Button("Start Session", ImVec2(250, 40)))
            {
                std::cout << "Starting session with code: " << gen_code << std::endl;
                // TODO: Start WebSocket server here
                // CreateWebSocketServer(gen_code);
                ImGui::CloseCurrentPopup();
                session_state = 0; // Reset for next time
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button("Back", ImVec2(250, 30)))
            {
                session_state = 0;
            }
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(2);
}

void GUI::newFrame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    initialOption();

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
    // ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[18]);
    ImNodes::BeginNodeEditor();
    
    // ========== Node Menu ==========
    const bool open_menu = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        ImNodes::IsEditorHovered() && 
        ImGui::IsMouseClicked(ImGuiMouseButton_Right, false) && 
        selected_nodes.empty();

    if (open_menu) {
        ImGui::OpenPopup("add node");
    }
    
    NodeMenu Menu;
    if (Menu.Draw()) {
        ImVec2 position = Menu.GetClickPos();
        std::unique_ptr<NodeBase> node = Menu.CreateNode(Menu.GetNodeType(), m_renderer);
        if (node) {
            ImNodes::SetNodeScreenSpacePos(node->GetId(), position);
            n_nodes.push_back(std::move(node));
        }
    }

    // ========== Draw Nodes ==========
    selected_nodes.clear();
    selected_links.clear();

    for (const auto& node : n_nodes) {
        node->Draw();
        
        if (ImNodes::IsNodeSelected(node->GetId())) {
            selected_nodes.insert(node->GetId());
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right, false)) {
                ImGui::OpenPopup((node->GetInternalTitle() + "_" + std::to_string(node->GetId())).c_str());
            }
        }
        
        node->Description();
    }

    // ========== Draw Links ==========
    for (const Link& link : n_links) {
        ImNodes::Link(link.id, link.init_attr, link.end_attr);

        if (ImNodes::IsLinkSelected(link.id)) {
            selected_links.insert(link.id);
        }
    }

    // ========== Minimap & End Editor ==========
    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    ImNodes::EndNodeEditor();

    // ========== IMPORTANT: Handle Link Creation/Destruction AFTER EndNodeEditor() ==========
    
    // Helper Lambda
    auto findNodeByAttr = [&](int attr_id) -> NodeBase* {
        for (auto& node : n_nodes) {
            if (node->GetOutputId() == attr_id || node->GetInputId() == attr_id)
                return node.get();
        }
        return nullptr;
    };

    // Handle Link Creation
    int start_attr, end_attr;
    bool snap_create = true;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr, &snap_create)) {
        NodeBase* dst = findNodeByAttr(end_attr);
        if (auto* downloadNode = dynamic_cast<DownloadNode*>(dst)) {
            int input_id = downloadNode->GetInputId();
            bool already_connected = std::any_of(
                n_links.begin(), n_links.end(),
                [input_id](const Link& link) { return link.end_attr == input_id; }
            );
            if (already_connected) {
                // ImGui::PopFont();
                GUI::popStyle();
                ImGui::End();
                return;
            }
        }
        Link link;
        link.id = Link::link_next_id++;
        link.init_attr = start_attr;
        link.end_attr = end_attr;
        n_links.push_back(link);
    }

    int dropped_attr;
    if (ImNodes::IsLinkDropped(&dropped_attr)) {
        auto link_it = std::find_if(n_links.begin(), n_links.end(),
            [dropped_attr](const Link& link) { return link.end_attr == dropped_attr; });

        NodeBase* dst = (link_it != n_links.end()) ? findNodeByAttr(link_it->end_attr) : nullptr;

        auto clearPreview = [](NodeBase* node) {
            if (auto* downloadNode = dynamic_cast<DownloadNode*>(node)) {
                downloadNode->ClearPreview();
                downloadNode->input_image = ImageData();
                downloadNode->output_image = ImageData();
            } else if (auto* previewNode = dynamic_cast<PreviewNode*>(node)) {
                previewNode->ClearPreview();
                previewNode->input_image = ImageData();
                previewNode->output_image = ImageData();
            }
        };

        if (dst) {
            clearPreview(dst);
        } else {
            for (auto& node : n_nodes) {
                clearPreview(node.get());
            }
        }
    }

    // Handle Link Destruction
    int link_id;
    if (ImNodes::IsLinkDestroyed(&link_id)) {
        // Remove the destroyed link
        n_links.erase(
            std::remove_if(n_links.begin(), n_links.end(),
                [link_id](const Link& link) { return link.id == link_id; }),
            n_links.end()
        );
    }

    // ========== Process Nodes (Dataflow) ==========
    // First pass: process all nodes with their current input
    for (auto& node : n_nodes) {
        node->Process();
    }

    // Second pass: propagate data through links
    for (const Link& link : n_links) {
        NodeBase* src = findNodeByAttr(link.init_attr);
        NodeBase* dst = findNodeByAttr(link.end_attr);
        if (src && dst) {
            dst->input_image = src->output_image;
            dst->Process();
        }
    }

    // ========== Handle Node/Link Deletion ==========
    if (ImGui::IsKeyPressed(ImGuiKey_Delete, false)) {
        // Mark selected nodes for deletion
        for (int node_id : selected_nodes) {
            death_node.insert(node_id);
        }

        // Mark selected links for deletion
        for (int link_id : selected_links) {
            death_link.insert(link_id);
        }
    }

    // Delete nodes and their connected links
    if (!death_node.empty()) {
        // First, find all links connected to nodes being deleted
        for (int node_id : death_node) {
            // Find the node to get its input/output IDs
            auto node_it = std::find_if(n_nodes.begin(), n_nodes.end(),
                [node_id](const std::unique_ptr<NodeBase>& node) {
                    return node->GetId() == node_id;
                });
            
            if (node_it != n_nodes.end()) {
                int input_id = (*node_it)->GetInputId();
                int output_id = (*node_it)->GetOutputId();
                
                // Mark all links connected to this node for deletion
                for (const Link& link : n_links) {
                    if (link.init_attr == output_id || link.end_attr == input_id) {
                        death_link.insert(link.id);
                    }
                }
            }
        }
        
        // Delete the nodes
        certainDeathNode(n_nodes, death_node);
        death_node.clear();
    }

    // Delete links
    if (!death_link.empty()) {
        certainDeathLink(n_links, death_link);
        death_link.clear();
    }

    // === After deletion, update preview/download nodes if disconnected ===
    for (auto& node : n_nodes) {
        bool is_preview = dynamic_cast<PreviewNode*>(node.get());
        bool is_download = dynamic_cast<DownloadNode*>(node.get());
        if (is_preview || is_download) {
            int input_id = node->GetInputId();
            // Check if any link connects to this node's input
            bool connected = std::any_of(n_links.begin(), n_links.end(),
                [input_id](const Link& link) { return link.end_attr == input_id; });
            if (!connected) {
                // Clear preview and reset images
                if (is_preview) {
                    auto* previewNode = static_cast<PreviewNode*>(node.get());
                    previewNode->ClearPreview();
                    previewNode->input_image = ImageData();
                    previewNode->output_image = ImageData();
                } else if (is_download) {
                    auto* downloadNode = static_cast<DownloadNode*>(node.get());
                    downloadNode->ClearPreview();
                    downloadNode->input_image = ImageData();
                    downloadNode->output_image = ImageData();
                }
            }
        }
    }

    // ========== Cleanup ==========
    // ImGui::PopFont();
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
                // Only remove if node is marked for deletion AND is deletable
                return death_node.count(node->GetId()) > 0 && node->IsDeletable();
            }),
        n_nodes.end()
    );
}

inline void GUI::certainDeathLink(std::vector<Link>& n_links, std::unordered_set<int>& death_link){
    n_links.erase(
        std::remove_if(n_links.begin(), n_links.end(),
        [&death_link](const Link& link) {
            return death_link.count(link.id) > 0;
        }),
    n_links.end());
}

std::string generate_unique_code() {
    static const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    static std::mt19937_64 rng((std::random_device())());
    std::uniform_int_distribution<std::size_t> dist(0, sizeof(chars) - 2);

    auto gen_part = [&](int len) {
        std::string s;
        s.reserve(len);
        for (int i = 0; i < len; ++i) s += chars[dist(rng)];
        return s;
    };

    // 4 chars, dash, 5 chars (e.g. "a764-bhsk8")
    return gen_part(4) + "-" + gen_part(5);
}

void CreateWebSocketServer(){

}

void JoinWebSocketServer() {

}
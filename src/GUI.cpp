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
    io.IniFilename = nullptr;
    
    // Load fonts from preloaded virtual filesystem
    // The --preload-file maps assets/ to /assets in the virtual FS
    ImFont* font = io.Fonts->AddFontFromFileTTF("/assets/fonts/inter/Inter_18pt-Black.ttf", 18.0f);
    if (!font) {
        std::cout << "Warning: Failed to load custom font, using default" << std::endl;
        io.Fonts->AddFontDefault();
    } else {
        std::cout << "Custom font loaded successfully!" << std::endl;
        // Load all your other fonts here
        io.Fonts->AddFontFromFileTTF("/assets/fonts/inter/Inter_18pt-Bold.ttf", 18.0f);
        io.Fonts->AddFontFromFileTTF("/assets/fonts/inter/Inter_18pt-ExtraBold.ttf", 18.0f);
    }

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_window, m_gl_context);
    
    ImGui_ImplOpenGL3_Init("#version 300 es");

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
    // Use default font for web build
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImNodes::BeginNodeEditor();

    // ========== Node Menu ==========
    const bool open_menu = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        ImNodes::IsEditorHovered() && 
        ImGui::IsMouseClicked(ImGuiMouseButton_Right, false) && 
        selected_nodes.empty();

    if (open_menu) ImGui::OpenPopup("add node");

    if (m_nodeMenu.Draw()) {
        ImVec2 position = m_nodeMenu.GetClickPos();
        std::unique_ptr<NodeBase> node = m_nodeMenu.CreateNode(m_nodeMenu.GetNodeType());
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

        bool selected = ImNodes::IsNodeSelected(node->GetId());
        if (selected) {
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

    // ========== Cleanup ==========
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

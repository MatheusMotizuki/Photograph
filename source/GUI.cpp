#include "GUI.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE2_IMPLEMENTATION
#include "stb_image_resize2.h"

#include <random>
#include <string>
#include <cstring>

// function prototypes
<<<<<<< Updated upstream
SDL_Texture* LoadCursorImage(SDL_Renderer* renderer, const char* filename, int& out_width, int& out_height);

// cursor related
int cursor_width = 0;
int cursor_height = 0;
SDL_Texture* cursor_texture = nullptr;
=======
int ConnectAndCreateSession(std::string route, std::string roomID);
int ConnectAndJoinSession(std::string route, std::string roomID);

WebSocketClient* g_wsClient = nullptr;
std::string GUI::unique_code = "";
>>>>>>> Stashed changes

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

void GUI::initialOption()
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
                if (GUI::unique_code.empty()) // Only generate if not already set
                    GUI::unique_code = GUI::generate_unique_code();
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
<<<<<<< Updated upstream
                this->ConnectAndJoinSession("ws://localhost:58058/ws", session_code);
=======
                ConnectAndJoinSession("ws://localhost:58058/ws", session_code);
>>>>>>> Stashed changes
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndDisabled();
        }
        else if (session_state == 1)
        {
            ImGui::Text("Your session code:");
            ImGui::Spacing();

            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Use default font or monospace if available
            ImGui::SetNextItemWidth(250);

            char gen_buf[64];
            std::strncpy(gen_buf, GUI::unique_code.c_str(), sizeof(gen_buf));
            gen_buf[sizeof(gen_buf) - 1] = '\0';
            ImGui::InputText("##gen_code", gen_buf, sizeof(gen_buf), ImGuiInputTextFlags_ReadOnly);
            ImGui::PopFont();

            ImGui::Spacing();
            
            if (ImGui::Button("Copy to Clipboard", ImVec2(250, 40)))
            {
                ImGui::SetClipboardText(GUI::unique_code.c_str());
            }

            if (ImGui::Button("Start Session", ImVec2(250, 40)))
            {
<<<<<<< Updated upstream
                this->ConnectAndCreateSession("ws://localhost:58058/ws", GUI::unique_code);
=======
                std::cout << "Starting session with code: " << GUI::unique_code << std::endl;
                ConnectAndCreateSession("ws://localhost:58058/ws", GUI::unique_code);
>>>>>>> Stashed changes
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

void GUI::InitializeCallbacks() {
    if (wsClient) {
        wsClient->setNodeCreatedCallback([this](int node_id, int node_type, ImVec2 pos) {
            std::unique_ptr<NodeBase> node = NodeMenu().CreateNode(static_cast<NodeMenu::NodeType>(node_type), m_renderer);
            if (node) {
                ImNodes::SetNodeScreenSpacePos(node->GetId(), pos);
                n_nodes.push_back(std::move(node));
            }
        });

        wsClient->setLinkCreatedCallback([this](int link_id, int start_attr, int end_attr) {
            n_links.push_back({link_id, start_attr, end_attr});
        });

        wsClient->setMouseMovedCallback([this](const std::string& sender, ImVec2 pos) {
            remoteMousePositions[sender] = pos;
        });

        wsClient->setSelectedNodeCallback([this](int node_id, ImVec2 pos) {
            selected_nodes.insert(node_id);
            ImNodes::SetNodeScreenSpacePos(node_id, pos);
        });

        wsClient->setNodeDeletedCallback([this](int node_id) {
            n_nodes.erase(
                std::remove_if(n_nodes.begin(), n_nodes.end(),
                    [node_id](const std::unique_ptr<NodeBase>& node){
                        return node->GetId() == node_id;
                    }),
                n_nodes.end()
            );
        });

        wsClient->setLinkDeletedCallback([this](int link_id) {
            n_links.erase(
                std::remove_if(n_links.begin(), n_links.end(),
                    [link_id](const Link& link){
                        return link.id == link_id;
                    }),
                n_links.end()
            );
        });

        wsClient->setImageUploadCallback([this](std::string id, int width, int height, int channels) {
            try {
                std::lock_guard<std::mutex> lock(m_pendingImagesMtx);
                m_pendingImages.push_back({id, width, height, channels});
            } catch (const std::exception& e) {
                std::cerr << "Image download failed: " << e.what() << std::endl;
            }
        });
    }
}

void GUI::newFrame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
        std::lock_guard<std::mutex> lock(m_pendingImagesMtx);
        for (const auto& pending : m_pendingImages) {
            try {
                curlpp::Cleanup myCleanup;
                curlpp::Easy request;

                std::string download_url = "http://localhost:58058/download/" + pending.id + "/" + GUI::unique_code;
                request.setOpt(curlpp::options::Url(download_url));

                std::ostringstream response_stream;
                request.setOpt(curlpp::options::WriteStream(&response_stream));
                request.perform();
                std::string image_data = response_stream.str();

                int img_width = 0, img_height = 0, img_channels = 0;
                unsigned char* img_pixels = stbi_load_from_memory(
                    reinterpret_cast<const unsigned char*>(image_data.data()),
                    image_data.size(),
                    &img_width, &img_height, &img_channels, 0
                );

                if (img_pixels) {
                    for (auto& node : n_nodes) {
                        if (auto* inputNode = dynamic_cast<InputNode*>(node.get())) {
                            // Free previous resources
                            if (inputNode->GetTexture()) {
                                SDL_DestroyTexture(inputNode->GetTexture());
                                inputNode->SetTexture(nullptr);
                            }
                            if (inputNode->GetImageData()) {
                                stbi_image_free(inputNode->GetImageData());
                                inputNode->SetImageData(nullptr);
                            }

                            // Clear output_image
                            inputNode->output_image.pixels.clear();
                            inputNode->output_image.width = 0;
                            inputNode->output_image.height = 0;
                            inputNode->output_image.channels = 0;

                            // Allocate new image data
                            size_t img_size = img_width * img_height * img_channels;
                            unsigned char* new_image_data = (unsigned char*)malloc(img_size);
                            
                            if (new_image_data) {
                                memcpy(new_image_data, img_pixels, img_size);

                                // Create SDL surface and texture
                                SDL_Surface* surface = nullptr;
                                if (img_channels == 4) {
                                    surface = SDL_CreateRGBSurfaceFrom(
                                        (void*)new_image_data, img_width, img_height, 32, img_width * 4,
                                        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
                                } else if (img_channels == 3) {
                                    surface = SDL_CreateRGBSurfaceFrom(
                                        (void*)new_image_data, img_width, img_height, 24, img_width * 3,
                                        0x000000ff, 0x0000ff00, 0x00ff0000, 0);
                                }

                                if (surface) {
                                    SDL_Texture* new_texture = SDL_CreateTextureFromSurface(inputNode->GetRenderer(), surface);
                                    SDL_FreeSurface(surface);

                                    if (new_texture) {
                                        inputNode->SetTexture(new_texture);
                                        inputNode->SetTexWidth(img_width);
                                        inputNode->SetTexHeight(img_height);
                                        inputNode->SetImageData(new_image_data);

                                        // Update output_image
                                        inputNode->output_image.width = img_width;
                                        inputNode->output_image.height = img_height;
                                        inputNode->output_image.channels = img_channels;
                                        inputNode->output_image.pixels.assign(
                                            img_pixels, img_pixels + img_width * img_height * img_channels
                                        );
                                        inputNode->SetImageLoaded(true);
                                        inputNode->SetNewImageUploaded(false);
                                    } else {
                                        free(new_image_data);
                                    }
                                } else {
                                    free(new_image_data);
                                }
                            }
                            break;
                        }
                    }
                    stbi_image_free(img_pixels);
                }
            } catch (const std::exception& e) {
                std::cerr << "Image processing failed: " << e.what() << std::endl;
            }
        }
        m_pendingImages.clear();
    }

    initialOption();

    // this will be executed regardless of the 
    // connection state of the user, but it will 
    // only work if it is connected to the server

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
            if (wsClient) wsClient->newNode(GUI::unique_code, node->GetId(), position, static_cast<int>(Menu.GetNodeType()));
            n_nodes.push_back(std::move(node));
            // upon node creation send to server information about the node, the ID and the position
            // SendNodeToServer(node->GetId(), Menu.GetNodeType(), position);
        }
    }
    
    if (wsClient) {
        wsClient->setNodeCreatedCallback([this](int node_id, int node_type, ImVec2 pos) {
            std::unique_ptr<NodeBase> node = NodeMenu().CreateNode(static_cast<NodeMenu::NodeType>(node_type), m_renderer);
            if (node) {
                ImNodes::SetNodeScreenSpacePos(node->GetId(), pos);
                n_nodes.push_back(std::move(node));
            }
        });
    }

    // ========== Draw Nodes ==========
    selected_nodes.clear();
    selected_links.clear();

    for (const auto& node : n_nodes) {
        node->Draw();
        
        if (ImNodes::IsNodeSelected(node->GetId())) {
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                int id = node->GetId();
                ImVec2 pos = ImNodes::GetNodeScreenSpacePos(id);
                if (wsClient) wsClient->sendSelectedNode(GUI::unique_code, id, pos);
            }
            
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

    // ========== Server/Client communication ==========

    // this will only work when connected to a server
    if (wsClient){
        static ImVec2 prevMousePos = ImVec2(-1.0f, -1.0f);
        ImVec2 mousePos = ImGui::GetMousePos();

        if ((mousePos.x != prevMousePos.x || mousePos.y != prevMousePos.y) &&
            ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
            prevMousePos = mousePos;
            wsClient->sendMouse(GUI::unique_code, mousePos);
        }

        // Load cursor image once
        if (!cursor_texture) {
            cursor_texture = LoadCursorImage(m_renderer, "assets/images/triangle.png", cursor_width, cursor_height);
        }

        // Draw all remote cursors as images
        for (const auto& [userId, remotePos] : remoteMousePositions) {
            if (remotePos.x >= 0 && remotePos.y >= 0) {
                ImGui::GetForegroundDrawList()->AddImage(
                    (ImTextureID)cursor_texture,
                    ImVec2(remotePos.x, remotePos.y),
                    ImVec2(remotePos.x + cursor_width, remotePos.y + cursor_height)
                );
            }
        }
    }

    // ========== IMPORTANT: Handle Link Creation/Destruction AFTER EndNodeEditor() ==========

    // Collect all information
    // and send to ws server
    // if(!g_wsClient) { // change this to true
    //     if (ImNodes::NumSelectedNodes() > 0) {
    //         std::vector<int> selected_ids(ImNodes::NumSelectedNodes());
    //         ImNodes::GetSelectedNodes(selected_ids.data());
    //         for (int node_id : selected_ids) {
    //             selected_nodes.insert(node_id);
    //             std::cout << "Selected node: " << node_id << std::endl;
    //         }
    //     }

    //     // Print mouse position whenever inside the main window
    //     if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
    //         ImVec2 mousePos = ImGui::GetMousePos();
    //         std::cout << "Mouse Position: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
    //         // Move this inside an ImGui window to display
    //         ImGui::Begin("##mouse imformation", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_Tooltip);
    //         ImGui::Text("someone");
    //         ImGui::End();
    //     }
    // }

    if (g_wsClient){
        // Print mouse position whenever inside the main window
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
            ImVec2 mousePos = ImGui::GetMousePos();
            g_wsClient->sendMouse(GUI::unique_code, mousePos);
        }

        ImVec2 remotePos = g_wsClient->getRemoteMousePos();
        if (remotePos.x >= 0 && remotePos.y >= 0 && std::isfinite(remotePos.x) && std::isfinite(remotePos.y)) {
            ImGui::SetNextWindowPos(remotePos, ImGuiCond_Always);
            ImGui::Begin("Remote Mouse", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_Tooltip);
            ImGui::Text("Remote user");
            ImGui::End();
        }
    }
    
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
        // send to the server the information about link creation
<<<<<<< Updated upstream
        if (wsClient) wsClient->newLink(GUI::unique_code, link.id, link.init_attr, link.end_attr);
=======
>>>>>>> Stashed changes
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
        // send to the server this also, abou link drop
    }

    // Handle Link Destruction
    int link_id;
    if (ImNodes::IsLinkDestroyed(&link_id)) {
        // Remove the destroyed link
        if (wsClient) wsClient->sendDeadLink(GUI::unique_code, link_id);

        n_links.erase(
            std::remove_if(n_links.begin(), n_links.end(),
                [link_id](const Link& link) { return link.id == link_id; }),
            n_links.end()
            // send to server the link deletion
        );
    }

    // ========== Process Nodes (Dataflow) ==========
    // First pass: process all nodes with their current input
    for (auto& node : n_nodes) {
        node->Process();
        
        
        if (wsClient) {
            if (auto* inputNode = dynamic_cast<InputNode*>(node.get())) {
                const ImageData& img = inputNode->output_image;
                if (inputNode->NewImageUploaded && !img.pixels.empty() && img.width > 0 && img.height > 0 && img.channels > 0) {
                    wsClient->sendImage(GUI::unique_code, img.pixels, img.width, img.height, img.channels);
                    inputNode->NewImageUploaded = false;
                }
            }
        }
    }

    // Second pass: propagate data through links
    for (const Link& link : n_links) {
        NodeBase* src = findNodeByAttr(link.init_attr);
        NodeBase* dst = findNodeByAttr(link.end_attr);
        if (src && dst) {
            dst->input_image = src->output_image;
            dst->Process();
            // send to server when this happens
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

        // send to server both, ig?
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
            [this, &death_node](const std::unique_ptr<NodeBase>& node){
                // Only remove if node is marked for deletion AND is deletable
                if (node->IsDeletable() && death_node.count(node->GetId()) > 0) {
                    if (wsClient) wsClient->sendDeadNode(GUI::unique_code, node->GetId());
                    return true;
                }
                return false;
            }),
        n_nodes.end()
    );
}

inline void GUI::certainDeathLink(std::vector<Link>& n_links, std::unordered_set<int>& death_link){
    n_links.erase(
        std::remove_if(n_links.begin(), n_links.end(),
        [this, &death_link](const Link& link) {
            return death_link.count(link.id) > 0;
        }),
    n_links.end());
}

std::string GUI::generate_unique_code() {
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

<<<<<<< Updated upstream
int GUI::ConnectAndCreateSession(std::string route, std::string roomID) {
    if (!wsClient) {
        wsClient = new WebSocketClient(route);
        wsClient->create(roomID);
        this->InitializeCallbacks();
    }
    GUI::unique_code = roomID;
    return 0;
}

int GUI::ConnectAndJoinSession(std::string route, std::string roomID) {
    if (!wsClient) {
        wsClient = new WebSocketClient(route);
        wsClient->join(roomID);
        this->InitializeCallbacks();
    }
    GUI::unique_code = roomID;
    return 0;
}

SDL_Texture* LoadCursorImage(SDL_Renderer* renderer, const char* filename, int& out_width, int& out_height) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4); // Force RGBA
    if (!data) return nullptr;

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(
        data, width, height, 32, width * 4, SDL_PIXELFORMAT_RGBA32);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    out_width = width;
    out_height = height;

    SDL_FreeSurface(surface);
    stbi_image_free(data);

    return texture;
=======
int ConnectAndCreateSession(std::string route, std::string roomID) {
    if (!g_wsClient) {
        g_wsClient = new WebSocketClient(route);
        g_wsClient->create(roomID);
    }
    return 0;
}

int ConnectAndJoinSession(std::string route, std::string roomID) {
    if (!g_wsClient) {
        g_wsClient = new WebSocketClient(route);
        g_wsClient->join(roomID);
    }
    return 0;
>>>>>>> Stashed changes
}
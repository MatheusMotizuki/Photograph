#include "node/submodules/io/Input.hpp"

InputNode::InputNode(SDL_Renderer* renderer) 
    : NodeBase("Input Node", PinType::Output, "input_node", false, ImVec4(0.2f, 0.7f, 1.0f, 1.0f))
    , m_renderer(renderer) 
{
    ImNodes::SetNodeScreenSpacePos(GetId(), ImVec2(60, 50));
    std::cout << "creating input node" << std::endl;
}

InputNode::~InputNode() {
    std::cout << "destroying input node" << std::endl;
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    if (m_image_data) {
        stbi_image_free(m_image_data);
        m_image_data = nullptr;
    }
}

bool InputNode::ShouldDisplayText() const { return false; }

void InputNode::NodeContent() {
    ImGui::TextDisabled("Input Preview:");
    setStyle();

    static bool openPicker = false;
    static int width = 0, height = 0, og_chans = 0;

    if (openPicker) {
        filePicker.ShowFileDialog(&openPicker);

        if (m_texture) {
            SDL_DestroyTexture(m_texture);
            m_texture = nullptr;
        }
        if (m_image_data) {
            stbi_image_free(m_image_data);
            m_image_data = nullptr;
        }

        if (filePicker.GetSelectedFile()) {
            m_image_data = stbi_load(filePicker.selected_file, &width, &height, &og_chans, 0);
        }

        // if image data was loaded successfully
        if (m_image_data) {
            SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
                (void*)m_image_data, width, height, og_chans * 8, og_chans * width, 
                0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
            m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
            SDL_FreeSurface(surface);
        } else {
            ImGui::Text("Failed to load image");
        }
    }

    // if we have a texture and image_data
    if (m_texture && m_image_data) {
        float m_width = 200.f, m_height = 200.f; // cap the image size
        float image_w = (float)width, image_h = (float)height;
        float scale = 1.0f;
        ImVec2 size;

        if (image_w > m_width) { // simple check to display the image better
            scale = m_width / image_w;
            size = ImVec2(m_width, image_h * scale);
        } else if (image_h > m_height) {
            scale = m_height / image_h;
            size = ImVec2(image_w * scale, m_height);
        } else {
            size = ImVec2(image_w, image_h);
        }

        ImGui::BeginChild("##input image", ImVec2(m_width, size.y), false);
        ImGui::Image((ImTextureID)(intptr_t)m_texture, size);
        ImGui::EndChild();
    }

    if (ImGui::Button("Upload image", ImVec2(200, 30))) {
        openPicker = true;
    }

    popStyle();
}

void InputNode::setStyle() {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(18, 18, 18, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(61, 61, 61, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(31, 31, 31, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 160, 200, 255));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[15]);
}

void InputNode::popStyle() {
    ImGui::PopFont();
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);
}

void InputNode::Process() {
    
}

void InputNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.2f, 0.7f, 1.0f, 1.0f), "Input Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Loads an image from your computer");
        ImGui::Text("and provides it as the starting point");
        ImGui::Text("for your node graph.");
        ImGui::Spacing();
        ImGui::Text("Supported formats include PNG, JPG,");
        ImGui::Text("and other common image types.");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Use this node to import");
        ImGui::TextDisabled("photos or textures for editing.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}
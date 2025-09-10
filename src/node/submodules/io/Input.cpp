#include "node/submodules/io/Input.hpp"

InputNode::InputNode(SDL_Renderer* renderer) 
    : NodeBase("Input Node", PinType::Output, false, ImVec4(0.2f, 0.7f, 1.0f, 1.0f))
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
}

bool InputNode::ShouldDisplayText() const { return false; }

void InputNode::NodeContent() {
    setStyle();

    static bool openPicker = false;
    static int width = 0, height = 0, og_chans = 0;
    static unsigned char* image_data = nullptr;
    static SDL_Texture* texture;

    if (openPicker) {
        filePicker.ShowFileDialog(&openPicker);
        if (image_data) { stbi_image_free(image_data); image_data = nullptr; }
        if (texture) { SDL_DestroyTexture(texture); texture = nullptr; }
        image_data = stbi_load(filePicker.selected_file, &width, &height, &og_chans, 0);

        // if image data was loaded successfully
        if (image_data) {
            texture = SDL_CreateTextureFromSurface(m_renderer, SDL_CreateRGBSurfaceFrom(
                (void*)image_data, width, height, og_chans * 8, og_chans * width, 
                0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000));
        }
    }

    // if we have a texture and image_data
    if (texture && image_data) {
        float m_width = 300.f, m_height = 200.f;
        float image_w = (float)width, image_h = (float)height;
        float scale = std::min(m_width / image_w, m_height / image_h);
        ImVec2 size(image_w * scale, image_h * scale);

        ImGui::BeginChild("##input image", ImVec2(m_width, m_height), false);
        ImGui::Image((ImTextureID)(intptr_t)texture, size);
        ImGui::EndChild();
    }

    if (ImGui::Button("Upload image", ImVec2(300, 30))) {
        openPicker = true;
    }

    popStyle();
}

void InputNode::setStyle() {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
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
    ImGui::PopStyleVar(5);
}
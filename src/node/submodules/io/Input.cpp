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
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));

    ImGui::PopStyleVar(3);
    
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(18, 18, 18, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(61, 61, 61, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(31, 31, 31, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 160, 200, 255));

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[11]);

    if (ImGui::Button("Upload image", ImVec2(256, 30))) openDialog();

    ImGui::PopFont();

    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);
}

void InputNode::openDialog() {
    std::cout << "upload image button pressed" << std::endl;
}
#include "node/submodules/Brightness.hpp"

BrightnessNode::BrightnessNode() : NodeBase("Brightness Node", PinType::Both, true, ImVec4(0.6f, 0.4f, 0.9f, 1.0f)) {
    std::cout << "creating brightness node" << std::endl;
}

BrightnessNode::~BrightnessNode() {
    std::cout << "destroying brightness node" << std::endl;
}

unsigned int BrightnessNode::GetBorderColor() const {
    return IM_COL32(153, 102, 230, 255);
}

void BrightnessNode::NodeContent() {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
    // --
    ImGui::PopStyleVar(3);
}
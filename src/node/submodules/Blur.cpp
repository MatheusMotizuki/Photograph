#include "node/submodules/Blur.hpp"

BlurNode::BlurNode() : NodeBase("Blur Node", PinType::Both, true, ImVec4(1.0f, 0.37f, 0.0f, 1.0f)){
    std::cout << "Creating blur node" << std::endl;
}

BlurNode::~BlurNode() {
    std::cout << "destroying blur node" << std::endl;
}

unsigned int BlurNode::GetBorderColor() const {
    return IM_COL32(255, 94, 0, 255);
}

void BlurNode::NodeContent() {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
    // --
    ImGui::PopStyleVar(3);
}

void BlurNode::Process() {
    
}
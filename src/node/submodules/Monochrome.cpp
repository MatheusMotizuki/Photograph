#include "node/submodules/Monochrome.hpp"

MonochromeNode::MonochromeNode() : NodeBase("Monochrome Node", PinType::Both, true, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)){
    std::cout << "creating monochrome node" << std::endl;
}

MonochromeNode::~MonochromeNode() {
    std::cout << "destroying monochrome node" << std::endl;
}

unsigned int MonochromeNode::GetBorderColor() const {
    return IM_COL32(0, 255, 255, 255);
}

void MonochromeNode::NodeContent() {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
    // --
    ImGui::PopStyleVar(3);
}
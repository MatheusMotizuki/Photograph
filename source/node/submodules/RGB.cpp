#include "node/submodules/RGB.hpp"

RGBNode::RGBNode() : NodeBase("RGB Node", PinType::Both, "rgb_node", true, ImVec4(0.2f, 0.6f, 0.9f, 1.0f)) {}
RGBNode::~RGBNode() {}

unsigned int RGBNode::GetBorderColor() const {
    return IM_COL32(51, 153, 230, 255);
}

void RGBNode::NodeContent() {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
    // --
    
    ImGui::PopStyleVar(3);
    
    static int r = 128, g = 128, b = 128;
    ImGui::SetNextItemWidth(80);
    ImGui::SliderInt("R", &r, 0, 255);
    ImGui::SetNextItemWidth(80);
    ImGui::SliderInt("G", &g, 0, 255);
    ImGui::SetNextItemWidth(80);
    ImGui::SliderInt("B", &b, 0, 255);
}

void RGBNode::Process() {

}

void RGBNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.2f, 0.6f, 0.9f, 1.0f), "RGB Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Extracts or manipulates the Red, Green, and Blue channels of the input image.");
        ImGui::Text("This node allows you to work directly with the RGB color components,");
        ImGui::Text("enabling color adjustments, channel isolation, or recombination.");
        ImGui::Spacing();
        ImGui::BulletText("Input: Image");
        ImGui::BulletText("Output: Image with modified or extracted RGB channels");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Use this node to adjust color balance or isolate specific channels.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}
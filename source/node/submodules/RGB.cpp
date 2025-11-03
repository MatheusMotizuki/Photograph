#include "node/submodules/RGB.hpp"

RGBNode::RGBNode() : NodeBase("RGB Node", PinType::Both, "rgb_node", true, ImVec4(0.2f, 0.6f, 0.9f, 1.0f)) {}
RGBNode::~RGBNode() {}

unsigned int RGBNode::GetBorderColor() const {
    return IM_COL32(51, 153, 230, 255);
}

void RGBNode::NodeContent() {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
    
    ImGui::SetNextItemWidth(80);
    ImGui::SliderInt("R", &m_r, -128, 128, "R %d");
    ImGui::SetNextItemWidth(80);
    ImGui::SliderInt("G", &m_g, -128, 128, "G %d");
    ImGui::SetNextItemWidth(80);
    ImGui::SliderInt("B", &m_b, -128, 128, "B %d");
    
    ImGui::PopStyleVar(3);
}

void RGBNode::ProcessInternal() {
    if (!input_image.isValid()) return;

    output_image = input_image;
    int channels = output_image.channels;
    for (int i = 0; i < output_image.width * output_image.height; ++i) {
        unsigned char* pixel = &output_image.pixels[i * channels];
        pixel[0] = static_cast<unsigned char>(std::clamp(int(pixel[0]) + m_r, 0, 255));
        pixel[1] = static_cast<unsigned char>(std::clamp(int(pixel[1]) + m_g, 0, 255));
        pixel[2] = static_cast<unsigned char>(std::clamp(int(pixel[2]) + m_b, 0, 255));
    }
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
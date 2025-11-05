#include "node/submodules/Brightness.hpp"
#include <opencv2/core.hpp>

BrightnessNode::BrightnessNode() : NodeBase("Brightness Node", PinType::Both, "brightness_node", true, ImVec4(0.6f, 0.4f, 0.9f, 1.0f)) {
    SetProcessDelay(150.0f);
}
BrightnessNode::~BrightnessNode() {}

unsigned int BrightnessNode::GetBorderColor() const {
    return IM_COL32(153, 102, 230, 255);
}

void BrightnessNode::NodeContent() {
    ImVec4 nodeColor = ImVec4(0.6f, 0.4f, 0.9f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_FrameBg,             ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.15f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,      ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.25f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,       ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.45f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab,          ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.85f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive,    ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 1.0f));

    ImGui::PushItemWidth(150);
    int prev_brightness = m_brightness_amount;
    ImGui::SliderInt("##brightness_amount", &m_brightness_amount, 0, 100, "Amount: %d");
    ImGui::PopItemWidth();

    ImGui::PopStyleColor(5);
    
    if (prev_brightness != m_brightness_amount) {
        MarkNeedsReprocess();
    }
}

void BrightnessNode::ProcessInternal() {
    if (!input_image.isValid()) return;
    
    output_image = input_image;
    
    // Map 0-100 to -127 to +127
    double brightness = (m_brightness_amount - 50) * 2.55;
    
    // Create OpenCV Mat from image data
    cv::Mat img(output_image.height, output_image.width, 
                output_image.channels == 4 ? CV_8UC4 : CV_8UC3, 
                output_image.pixels.data());
    
    // Add brightness using OpenCV (vectorized, SIMD-optimized)
    img.convertTo(img, -1, 1.0, brightness);
}

void BrightnessNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.6f, 0.4f, 0.9f, 1.0f), "Brightness Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Adjusts the brightness of the input image");
        ImGui::Text("by scaling the intensity of each pixel.");
        ImGui::Text("Increasing the brightness makes the image lighter,");
        ImGui::Text("while decreasing it makes the image darker.");
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Use in combination with other color");
        ImGui::TextDisabled("adjustment nodes for best results.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}
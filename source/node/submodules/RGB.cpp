#include "node/submodules/RGB.hpp"
#include <opencv2/core.hpp>

RGBNode::RGBNode() : NodeBase("RGB Node", PinType::Both, "rgb_node", true, ImVec4(0.2f, 0.6f, 0.9f, 1.0f)) {}
RGBNode::~RGBNode() {}

unsigned int RGBNode::GetBorderColor() const {
    return IM_COL32(51, 153, 230, 255);
}

void RGBNode::NodeContent() {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
    
    ImGui::SetNextItemWidth(128);
    ImGui::SliderInt("R", &m_r, -128, 128, "R %d");
    ImGui::SetNextItemWidth(128);
    ImGui::SliderInt("G", &m_g, -128, 128, "G %d");
    ImGui::SetNextItemWidth(128);
    ImGui::SliderInt("B", &m_b, -128, 128, "B %d");
    
    ImGui::PopStyleVar(3);
}

void RGBNode::ProcessInternal() {
    if (!input_image.isValid()) return;

    output_image = input_image;
    
    // Create OpenCV Mat from image data
    cv::Mat img(output_image.height, output_image.width, 
                output_image.channels == 4 ? CV_8UC4 : CV_8UC3, 
                output_image.pixels.data());
    
    // Add scalar to each channel using OpenCV (vectorized operation)
    cv::Scalar adjustment(m_r, m_g, m_b, 0);
    cv::add(img, adjustment, img);
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
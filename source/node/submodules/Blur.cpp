#include "node/submodules/Blur.hpp"
#include <opencv2/imgproc.hpp>

BlurNode::BlurNode() : NodeBase("Blur Node", PinType::Both, "blur_node", true, ImVec4(1.0f, 0.37f, 0.0f, 1.0f)) {
    SetProcessDelay(100.0f); // Reduced delay since OpenCV is much faster
}
BlurNode::~BlurNode() {}

unsigned int BlurNode::GetBorderColor() const {
    return IM_COL32(255, 94, 0, 255);
}

void BlurNode::NodeContent() {
    static const char* blurTypes[] = { "Gaussian", "Box", "Median" };

    setStyleCombo();
    ImGui::PushItemWidth(150);
    int prev_blur_type = m_blur_type;
    ImGui::Combo("Blur Type", &m_blur_type, blurTypes, IM_ARRAYSIZE(blurTypes));
    ImGui::PopItemWidth();
    popStyleCombo();

    setStyleSlider();
    ImGui::PushItemWidth(150);
    int prev_blur_amount = m_blur_amount;
    ImGui::SliderInt("##blur_amount", &m_blur_amount, 0, 30, "Amount: %d");
    ImGui::PopItemWidth();
    popStyleSlider();
    
    if (prev_blur_amount != m_blur_amount || prev_blur_type != m_blur_type) {
        MarkNeedsReprocess();
    }
}

void BlurNode::setStyleCombo() {
    ImGui::PushStyleColor(ImGuiCol_FrameBg,          ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.15f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,   ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.25f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,    ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.45f));
    ImGui::PushStyleColor(ImGuiCol_Button,           ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.35f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,    ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.55f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,     ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.75f));
    ImGui::PushStyleColor(ImGuiCol_Header,           ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.35f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered,    ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.55f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive,     ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.75f));
}

void BlurNode::popStyleCombo() {
    ImGui::PopStyleColor(9);
}

void BlurNode::setStyleSlider() {
    ImGui::PushStyleColor(ImGuiCol_FrameBg,             ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.15f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,      ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.25f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,       ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.45f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab,          ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 0.85f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive,    ImVec4(nodeColor.x, nodeColor.y, nodeColor.z, 1.0f));
}

void BlurNode::popStyleSlider() {
    ImGui::PopStyleColor(5);
}

void BlurNode::ProcessInternal() {
    if (!input_image.isValid() || m_blur_amount == 0) {
        output_image = input_image;
        return;
    }
    
    output_image = input_image;
    
    // Create OpenCV Mat from image data
    cv::Mat img(output_image.height, output_image.width, 
                output_image.channels == 4 ? CV_8UC4 : CV_8UC3, 
                output_image.pixels.data());
    
    // Map blur amount to kernel size (must be odd)
    int kernel_size = (m_blur_amount / 5) * 2 + 1;
    kernel_size = std::max(3, std::min(kernel_size, 30)); // Clamp between 3 and 31
    
    // Apply appropriate blur using OpenCV (highly optimized with SIMD)
    switch (m_blur_type) {
        case 0: // Gaussian
            cv::GaussianBlur(img, img, cv::Size(kernel_size, kernel_size), 0);
            break;
        case 1: // Box
            cv::blur(img, img, cv::Size(kernel_size, kernel_size));
            break;
        case 2: // Median
            cv::medianBlur(img, img, kernel_size);
            break;
    }
}

void BlurNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 8.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(1.0f, 0.37f, 0.0f, 1.0f), "Blur Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Applies a configurable blur effect to the input image.");
        ImGui::Text("Softens details and reduces sharpness.");
        ImGui::Text("Useful for background effects,");
        ImGui::Text("noise reduction, or artistic looks.");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Combine with other nodes for advanced visual effects,");
        ImGui::TextDisabled("or use multiple Blur nodes for stronger results.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}
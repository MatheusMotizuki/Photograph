#include "node/submodules/Blur.hpp"

BlurNode::BlurNode() : NodeBase("Blur Node", PinType::Both, "blur_node", true, ImVec4(1.0f, 0.37f, 0.0f, 1.0f)) {
    SetProcessDelay(300.0f); // 300ms delay for blur (it's expensive)
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
    ImGui::SliderInt("##blur_amount", &m_blur_amount, 0, 100, "Amount: %d");
    ImGui::PopItemWidth();
    popStyleSlider();
    
    // Only mark for reprocess if values changed
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
    
    int radius = std::max(1, m_blur_amount / 5);
    int width = output_image.width;
    int height = output_image.height;
    int channels = output_image.channels;
    
    // Process only RGB channels (skip alpha at index 3)
    int process_channels = std::min(3, channels);
    
    // For each channel (excluding alpha), compute integral image and apply box filter
    for (int c = 0; c < process_channels; ++c) {
        // Build integral image for this channel
        std::vector<int> integral((width + 1) * (height + 1), 0);
        
        for (int y = 1; y <= height; ++y) {
            for (int x = 1; x <= width; ++x) {
                int pixel_val = input_image.pixels[((y-1) * width + (x-1)) * channels + c];
                integral[y * (width + 1) + x] = pixel_val +
                    integral[(y-1) * (width + 1) + x] +
                    integral[y * (width + 1) + (x-1)] -
                    integral[(y-1) * (width + 1) + (x-1)];
            }
        }
        
        // Apply box filter using integral image (O(1) per pixel!)
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int y1 = std::max(0, y - radius);
                int y2 = std::min(height - 1, y + radius);
                int x1 = std::max(0, x - radius);
                int x2 = std::min(width - 1, x + radius);
                
                // Convert to integral image coordinates (1-indexed)
                int sum = integral[(y2 + 1) * (width + 1) + (x2 + 1)] -
                          integral[y1 * (width + 1) + (x2 + 1)] -
                          integral[(y2 + 1) * (width + 1) + x1] +
                          integral[y1 * (width + 1) + x1];
                
                int count = (y2 - y1 + 1) * (x2 - x1 + 1);
                output_image.pixels[(y * width + x) * channels + c] = sum / count;
            }
        }
    }
    // Alpha channel (if exists) remains unchanged
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

#include "node/submodules/Brightness.hpp"

BrightnessNode::BrightnessNode() : NodeBase("Brightness Node", PinType::Both, "brightness_node", true, ImVec4(0.6f, 0.4f, 0.9f, 1.0f)) {
    SetProcessDelay(150.0f); // 150ms delay for brightness
}
BrightnessNode::~BrightnessNode() {}

unsigned int BrightnessNode::GetBorderColor() const {
    return IM_COL32(153, 102, 230, 255); // Matches ImVec4(0.6f, 0.4f, 0.9f, 1.0f)
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
    ImGui::SliderInt("Brightness", &m_brightness_amount, 0, 100, "Amount: %d");
    ImGui::PopItemWidth();

    ImGui::PopStyleColor(5);
    
    if (prev_brightness != m_brightness_amount) {
        MarkNeedsReprocess();
    }
}

void BrightnessNode::ProcessInternal() {
    if (!input_image.isValid() || input_image.pixels.empty()) {
        output_image = ImageData(); // Clear output if no input
        return;
    }
    
    output_image = input_image;
    
    int brightness = static_cast<int>((m_brightness_amount - 50) * 2.55f);
    int channels = output_image.channels;
    size_t pixel_count = output_image.width * output_image.height;
    
    for (size_t i = 0; i < pixel_count; ++i) {
        unsigned char* pixel = &output_image.pixels[i * channels];
        for (int c = 0; c < std::min(3, channels); ++c) {
            int val = static_cast<int>(pixel[c]) + brightness;
            pixel[c] = static_cast<unsigned char>(std::clamp(val, 0, 255));
        }
        // Alpha channel remains unchanged
    }
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

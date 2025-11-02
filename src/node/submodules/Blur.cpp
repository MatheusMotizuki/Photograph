#include "node/submodules/Blur.hpp"

BlurNode::BlurNode() : NodeBase("Blur Node", PinType::Both, "blur_node", true, ImVec4(1.0f, 0.37f, 0.0f, 1.0f)) {}
BlurNode::~BlurNode() {}

unsigned int BlurNode::GetBorderColor() const {
    return IM_COL32(255, 94, 0, 255);
}

void BlurNode::NodeContent() {
    static int blurAmount = 0;

    // Placeholder combo options
    static const char* blurTypes[] = { "Gaussian", "Box", "Median" };
    static int currentBlurType = 0;

    setStyleCombo();
    ImGui::PushItemWidth(150);
    ImGui::Combo("Blur Type", &currentBlurType, blurTypes, IM_ARRAYSIZE(blurTypes));
    ImGui::PopItemWidth();
    popStyleCombo();

    setStyleSlider();
    
    ImGui::PushItemWidth(150);
    ImGui::SliderInt("##blur_amount", &blurAmount, 0, 100, "Amount: %d");
    ImGui::PopItemWidth();

    popStyleSlider();
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

void BlurNode::Process() {
    
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
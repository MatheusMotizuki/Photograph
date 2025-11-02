#include "node/submodules/io/Preview.hpp"

PreviewNode::PreviewNode() 
    : NodeBase("Preview Node", PinType::Both, "preview_node", true, ImVec4(0.4f, 0.6f, 1.0f, 1.0f)) {}

PreviewNode::~PreviewNode() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    if (m_image_data) {
        stbi_image_free(m_image_data);
        m_image_data = nullptr;
    }
}

unsigned int PreviewNode::GetBorderColor() const {
    return IM_COL32(102, 153, 255, 255);
}

bool PreviewNode::ShouldDisplayText() const { return false; }

void PreviewNode::NodeContent() {
    ImGui::TextDisabled("Output Preview:");
    setStyle();

    popStyle();
}

void PreviewNode::setStyle() {
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 255, 180, 255));
#ifdef __EMSCRIPTEN__
    // Use default font on web
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
#else
    // Check if font exists before accessing
    if (ImGui::GetIO().Fonts->Fonts.Size > 15) {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[15]);
    } else {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    }
#endif
}

void PreviewNode::popStyle() {
    ImGui::PopFont();
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(2);
}

void PreviewNode::Process() {

}

void PreviewNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "Output Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Displays the final result of your node graph.");
        ImGui::Text("Use this node to preview the processed image.");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Connect your last processing node");
        ImGui::TextDisabled("to this node to see the final effect.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}
#include "node/submodules/io/Download.hpp"

DownloadNode::DownloadNode() 
    : NodeBase("Download Node", PinType::Input, "download_node", false, ImVec4(0.3137f, 1.0f, 0.7059f, 1.0f)) {
    ImNodes::SetNodeScreenSpacePos(GetId(), ImVec2(900, 350));
}

DownloadNode::~DownloadNode() {}

bool DownloadNode::ShouldDisplayText() const { return false; }

void DownloadNode::NodeContent() {
    setStyle();

    if (ImGui::Button("Download image", ImVec2(200, 30))) { 
        std::cout << "clicked" << std::endl;
        // TODO: implement this.
    }

    popStyle();
}

void DownloadNode::setStyle() {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(18, 18, 18, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(61, 61, 61, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(31, 31, 31, 255));
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

void DownloadNode::popStyle() {
    ImGui::PopFont();
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);
}

void DownloadNode::Process() {

}

void DownloadNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.31f, 1.0f, 0.71f, 1.0f), "Download Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Allows you to download the final output image.");
        ImGui::Text("Use this node to save the processed image to your device.");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Connect your last processing node");
        ImGui::TextDisabled("to this node, then click 'Download image'.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}
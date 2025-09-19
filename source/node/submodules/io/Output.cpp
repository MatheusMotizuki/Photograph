#include "node/submodules/io/Output.hpp"

OutputNode::OutputNode() 
    : NodeBase("Output Node", PinType::Input, "output_node", false, ImVec4(0.3137f, 1.0f, 0.7059f, 1.0f)) 
{
    ImNodes::SetNodeScreenSpacePos(GetId(), ImVec2(900, 350));
    std::cout << "creating output node" << std::endl; 
}

OutputNode::~OutputNode() {
    std::cout << "destroying output node" << std::endl;
}

bool OutputNode::ShouldDisplayText() const { return false; }

void OutputNode::NodeContent() {
    ImGui::TextDisabled("Output Preview:");
    setStyle();

    if (ImGui::Button("Download image", ImVec2(200, 30))) { 
        std::cout << "clicked" << std::endl;
        // TODO: implement this.
    }

    popStyle();
}

void OutputNode::setStyle() {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(18, 18, 18, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(61, 61, 61, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(31, 31, 31, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 255, 180, 255));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[15]);
}

void OutputNode::popStyle() {
    ImGui::PopFont();
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);
}

void OutputNode::Process() {

}

void OutputNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.31f, 1.0f, 0.71f, 1.0f), "Output Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Displays the final result of your node graph.");
        ImGui::Text("Use this node to preview and export the processed image.");
        ImGui::Spacing();
        ImGui::Text("Click 'Download image' to save the output to disk.");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Connect your last processing node");
        ImGui::TextDisabled("to this node to see the final effect.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}
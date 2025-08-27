#include "node/NodeMenu.hpp"

bool NodeMenu::Draw() {
    const bool new_node = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
    ImNodes::IsEditorHovered() && ImGui::IsMouseDown(1);

    if (new_node) ImGui::OpenPopup("add node");

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    bool created = false;

    if (ImGui::BeginPopup("add node")) {
        position = ImGui::GetMousePosOnOpeningCurrentPopup();

        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.75f, 1.0f), "Add a new node");
        ImGui::Separator();

        ImGui::Spacing();
        for (size_t i = 1; i + 1 < items.size(); ++i) {
            const auto& item = items[i];
            if (ImGui::MenuItem(item.label)) {
                nodeType = item.type;
                created = true;
                ImGui::CloseCurrentPopup();
                break;
            }
            ImGui::Spacing();
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);

    if (created) return true;

    return false;
}

NodeMenu::NodeType NodeMenu::GetNodeType() const { return nodeType; }
ImVec2 NodeMenu::GetClickPos() const { return position; }
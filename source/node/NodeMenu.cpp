#include "node/NodeMenu.hpp"

bool NodeMenu::Draw() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    bool created = false;

    if (ImGui::BeginPopup("add node")) {
        position = ImGui::GetMousePosOnOpeningCurrentPopup();

        ImGui::TextColored(ImVec4(0.2f, 0.6f, 1.0f, 1.0f), "Add a new node");
        ImGui::Separator();

        ImGui::Spacing();
        for (size_t i = 1; i + 1 < items.size(); ++i) {
            const auto& item = items[i];
            // Set a custom color for the menu item background when hovered/selected
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.196f, 0.196f, 0.196f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));

            if (ImGui::Selectable(item.label)) {
                nodeType = item.type;
                created = true;
                ImGui::PopStyleColor(3);
                break;
            }

            ImGui::PopStyleColor(3);
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

std::unique_ptr<NodeBase> NodeMenu::CreateNode(NodeMenu::NodeType type, SDL_Renderer* renderer) {
    switch (type)
    {
    case NodeMenu::NodeType::MonochromeNode:
        return std::make_unique<MonochromeNode>();
        break;
    case NodeMenu::NodeType::BrightnessNode:
        return std::make_unique<BrightnessNode>();
        break;
    case NodeMenu::NodeType::BlurNode:
        return std::make_unique<BlurNode>();
        break;
    case NodeMenu::NodeType::RGBNode:
        return std::make_unique<RGBNode>();
        break;
    case NodeMenu::NodeType::PreviewNode:
        return std::make_unique<PreviewNode>(renderer);
        break;
    default:
        return nullptr;
        break;
    }
}
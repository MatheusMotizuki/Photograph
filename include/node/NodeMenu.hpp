#pragma once

#include "imgui.h"
#include "imnodes.h"
#include <string>

class NodeMenu {
private:
    bool open = false;
    ImVec2 position = {0,0}; // dafault to 0,0 if for some reason it does not work

public:
    NodeMenu() = default;
    ~NodeMenu() = default;

    enum class NodeType{ // types of nodes
        InputNode,
        MonochromeNode,
        BlurNode,
        OutputNode,
    } nodeType;

    struct MenuItem {
        const char* label;
        NodeType type;
    };

    // Always put the new nodes in between the input and output
    std::vector<MenuItem> items{
        {"Input node", NodeType::InputNode},
        // --

        {"Monochrome node", NodeType::MonochromeNode},
        {"Blur node", NodeType::BlurNode},

        // --
        {"Output node", NodeType::OutputNode},
    };

    bool Draw() {
        const bool new_node = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        ImNodes::IsEditorHovered() && ImGui::IsMouseDown(1);

        if (new_node) {
            ImGui::OpenPopup("add node");
        }

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

    NodeType GetNodeType() const {
        return nodeType;
    }

    ImVec2 GetClickPos() const {
        return position;
    }
};
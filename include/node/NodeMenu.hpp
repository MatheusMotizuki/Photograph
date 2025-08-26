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

    enum class NodeType{
        RGB_break,
        RGB_unite,
        Monochrome,
        HUE,
    } nodeType;

    struct MenuItem {
        const char* label;
        NodeType type;
    };

    MenuItem menuItems[4] = {
        {"RGB break", NodeType::RGB_break},
        {"RGB unite", NodeType::RGB_unite},
        {"Monochrome", NodeType::Monochrome},
        {"HUE", NodeType::HUE}
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

            for (const auto& item : menuItems) {
                if (ImGui::MenuItem(item.label)) {
                    nodeType = item.type;
                    created = true;
                    ImGui::CloseCurrentPopup();
                    break;
                }
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
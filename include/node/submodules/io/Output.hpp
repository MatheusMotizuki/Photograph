#pragma once

#include "node/NodeBase.hpp"
#include <iostream>

class OutputNode : public NodeBase{
private:
// skip
protected:
    bool ShouldDisplayText() const override { return false; }
    void NodeContent() override {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
        
        ImGui::BeginChild("image output", ImVec2(256, 128));
        
        ImGui::EndChild();

        ImGui::PopStyleVar(3);

        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(18, 18, 18, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(61, 61, 61, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(31, 31, 31, 255));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 255, 180, 255));
    
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[11]);

        if (ImGui::Button("Download image", ImVec2(256, 30))) { openDialog(); }

        ImGui::PopFont();

        ImGui::PopStyleColor(5);
        ImGui::PopStyleVar(2);
    }

    void openDialog() {
        std::cout << "download image button pressed" << std::endl;
    }

public:
    OutputNode()
        : NodeBase("Output Node", PinType::Input, false, ImVec4(0.3137f, 1.0f, 0.7059f, 1.0f)) { std::cout << "creating output node" << std::endl; }
    // 
};
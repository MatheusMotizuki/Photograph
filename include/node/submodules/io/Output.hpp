#pragma once

#include "node/NodeBase.hpp"
#include <iostream>

class OutputNode : public NodeBase{
private:
// skip
protected:
    void NodeContent() override {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
        // --
        ImGui::PopStyleVar(3);
    }
public:
    OutputNode()
        : NodeBase("Output Node", PinType::Input, false) {
            std::cout << "creating output node" << std::endl;
        }
    // 
};
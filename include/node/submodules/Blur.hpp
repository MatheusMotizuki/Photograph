#pragma once

#include "node/NodeBase.hpp"
#include <iostream>

class BlurNode : public NodeBase {
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

    unsigned int GetBorderColor() const override {
        return IM_COL32(255, 128, 0, 255);
    }
public:
    BlurNode() : NodeBase("Blur Node", PinType::Both, true, 
                                ImVec4(1.0f, 0.5f, 0.0f, 1.0f)) {}
    //
};
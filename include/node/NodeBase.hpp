#pragma once

#include "imgui.h"
#include "imnodes.h"
#include "style.hpp"
#include <string>

#include <iostream>

enum class PinType { Input, Output, Both };

class NodeBase {
private:
    static int next_id;
    NodeStyle style;
    
    void SetNodeStyle(unsigned int borderColor = IM_COL32(58, 58, 58, 255)) {
        style.SetStyle(borderColor);
    }
    
    void ResetStyle() {
        style.PopStyle();
    }

protected:
    int n_id;
    std::string n_title;
    ImVec4 n_title_col;
    PinType n_pinType;
    int n_input_id;
    int n_output_id;
    bool deletable;

    // Virtual methods for customization
    virtual void NodeContent() = 0; // pure virtual, must be implemented
    virtual bool ShouldDisplayText() const { return true; }
    virtual unsigned int GetBorderColor() const { return IM_COL32(58, 58, 58, 255); }
    virtual ImFont* GetTitleFont() const { return ImGui::GetIO().Fonts->Fonts[12]; }

    // Common
    void InputText() {
        if (n_pinType == PinType::Input || n_pinType == PinType::Both) {
            ImNodes::BeginInputAttribute(n_input_id, ImNodesPinShape_CircleFilled);
            if (ShouldDisplayText()) ImGui::TextColored(ImVec4(0.702f, 0.702f, 0.702f, 1.f), "input"); else ImGui::Dummy(ImVec2(0, 0));
            ImNodes::EndInputAttribute();
        }
    }

    void OutputText() {
        if (n_pinType == PinType::Output || n_pinType == PinType::Both) {
            ImNodes::BeginOutputAttribute(n_output_id, ImNodesPinShape_CircleFilled);
            if (ShouldDisplayText()) ImGui::TextColored(ImVec4(0.702f, 0.702f, 0.702f, 1.f), "output"); else ImGui::Dummy(ImVec2(0, 0));
            ImNodes::EndOutputAttribute();
        }
    }

public:
    NodeBase(const std::string& title, PinType pinType = PinType::Both, 
             bool deletable = true, ImVec4 title_col = ImVec4(0.7f, 0.6f, 0.9f, 1.0f));

    void Draw() {
        SetNodeStyle(GetBorderColor());

        ImNodes::BeginNode(n_id);

        ImNodes::BeginNodeTitleBar();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[12]);
        ImGui::TextColored(n_title_col, "%s", n_title.c_str());
        ImGui::PopFont();
        ImNodes::EndNodeTitleBar();

        InputText();

        if(n_pinType == PinType::Both){
            ImGui::SameLine();
        }

        OutputText();

        NodeContent(); // this will come from the extended nodes

        ImNodes::EndNode();
        ResetStyle();
    }

    bool IsProtected() const { return deletable; }

    int GetId() const { return n_id; }

    bool IsSelected() const { return ImNodes::IsNodeSelected(n_id); }

    std::string GetTitle() const { return n_title; }

    virtual ~NodeBase() = default;
};
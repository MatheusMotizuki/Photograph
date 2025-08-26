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

    int n_id;
    std::string n_title;
    ImVec4 n_title_col;
    PinType n_pinType;

    int n_input_id;
    int n_output_id;

    bool deletable;
    
    NodeStyle style;

    void SetNodeStyle(){
        style.SetStyle();
    };

    void ResetStyle() {
        style.PopStyle();
    };

protected:
    virtual void NodeContent() {
        // Override this method to add custom content to the node
        // For example, you could add sliders, buttons, etc.
    }

public:
    NodeBase(const std::string& title, PinType pinType = PinType::Both, bool deletable = true, ImVec4 title_col = ImVec4(0.2f, 0.7f, 1.0f, 1.0f))
        : n_id(next_id++), n_title(title), n_pinType(pinType), deletable(deletable), n_title_col(title_col) {
        n_input_id = n_id * 10 + 1;   // Input pins end with 1
        n_output_id = n_id * 10 + 2;  // Output pins end with 2

        std::cout << "creating node with id: " << n_id << std::endl;
        std::cout << "creating node with in_id: " << n_input_id << std::endl;
        std::cout << "creating node with out_id: " << n_output_id << std::endl;
    }

    void Draw() {
        SetNodeStyle();

        ImNodes::BeginNode(n_id);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextColored(n_title_col, "%s", n_title.c_str());
        ImNodes::EndNodeTitleBar();

        if (n_pinType == PinType::Input || n_pinType == PinType::Both) {
            ImNodes::BeginInputAttribute(n_input_id, ImNodesPinShape_CircleFilled);
            ImGui::TextColored(ImVec4(0.702f, 0.702f, 0.702f, 1.f), "input");
            ImNodes::EndInputAttribute();
        }

        if(n_pinType == PinType::Both){
            ImGui::SameLine();
        }

        if (n_pinType == PinType::Output || n_pinType == PinType::Both) {
            ImNodes::BeginOutputAttribute(n_output_id, ImNodesPinShape_CircleFilled);
            ImGui::TextColored(ImVec4(0.702f, 0.702f, 0.702f, 1.f), "output");
            ImNodes::EndOutputAttribute();
        }

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
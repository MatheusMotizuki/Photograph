#pragma once

#include <imgui.h>
#include "imnodes.h"
#include "style.hpp"

#include <iostream>
#include <string>

enum class PinType { Input, Output, Both };

class NodeBase {
private:
    static int next_id;
    NodeStyle style;
    
    void SetNodeStyle(unsigned int borderColor);
    void ResetStyle();

protected:
    int n_id;
    std::string n_title;
    ImVec4 n_title_col;
    PinType n_pinType;
    int n_input_id;
    int n_output_id;
    bool deletable;
    std::string tooltip;

    // Virtual methods for customization
    virtual void NodeContent() = 0; // pure virtual, must be implemented
    virtual bool ShouldDisplayText() const;
    virtual unsigned int GetBorderColor() const;
    virtual ImFont* GetTitleFont() const;

    // Common
    void InputText();
    void OutputText();

public:
    NodeBase(const std::string& title, PinType pinType = PinType::Both, 
             bool deletable = true, ImVec4 title_col = ImVec4(0.7f, 0.6f, 0.9f, 1.0f), std::string tooltip = "Base Node");

    void Draw();

    bool IsProtected() const;
    int GetId() const;
    bool IsSelected() const;
    std::string GetTitle() const;
    virtual ~NodeBase() = default;
};

struct Link {
    int id;
    int init_attr;
    int end_attr;

    static int link_next_id;
};
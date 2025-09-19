#pragma once

#include <imgui.h>
#include "imnodes.h"
#include "style.hpp"

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_set>

enum class PinType { Input, Output, Both };

struct Link {
    int id;
    int init_attr;
    int end_attr;

    inline static int link_next_id = 1;
};

class NodeBase {
private:
    static int next_id;
    NodeStyle style;
    
    void SetNodeStyle(unsigned int borderColor);
    void ResetStyle();

protected:
    int n_id;
    int* n_id_ptr;
    std::string n_title;
    std::string n_internal_title;
    ImVec4 n_title_col;
    PinType n_pinType;
    int n_input_id;
    int n_output_id;
    bool deletable;

    virtual void Process() = 0; // where nodes modify incoming data
    virtual void NodeContent() = 0; // pure virtual, must be implemented

    // Virtual methods for customization
    virtual bool ShouldDisplayText() const;
    virtual unsigned int GetBorderColor() const;
    virtual ImFont* GetTitleFont() const;

    // Common
    void InputText();
    void OutputText();

public:
    NodeBase(const std::string& title, PinType pinType = PinType::Both, const std::string& internal_title = "base_node",
             bool deletable = true, ImVec4 title_col = ImVec4(0.7f, 0.6f, 0.9f, 1.0f));

    void Draw();

    bool IsProtected() const;
    int GetId() const;
    bool IsSelected() const;
    std::string GetInternalTitle() const;
    
    virtual ~NodeBase() = default;

    virtual void Description() = 0; // description for every node
};
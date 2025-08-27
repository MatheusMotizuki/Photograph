#pragma once

#include "imgui.h"
#include "imnodes.h"
#include <string>
#include <vector>

class NodeMenu {
private:
    bool open = false;
    ImVec2 position = {0,0};

public:
    NodeMenu() = default;
    ~NodeMenu() = default;

    enum class NodeType{ // types of nodes
        InputNode,
        MonochromeNode,
        BrightnessNode,
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
        {"Brightness node", NodeType::BrightnessNode},
        {"Blur node", NodeType::BlurNode},
        // --
        {"Output node", NodeType::OutputNode},
    };

    bool Draw();

    NodeType GetNodeType() const;
    ImVec2 GetClickPos() const;
};
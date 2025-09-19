#pragma once

#include "imgui.h"
#include "imnodes.h"
#include <string>
#include <vector>

#include "node/NodeBase.hpp"

// In 'n' out
#include "node/submodules/io/Input.hpp"
#include "node/submodules/io/Output.hpp"
// Common
#include "node/submodules/Monochrome.hpp"
#include "node/submodules/Brightness.hpp"
#include "node/submodules/Blur.hpp"

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
        {"Monochrome", NodeType::MonochromeNode},
        {"Brightness", NodeType::BrightnessNode},
        {"Blur", NodeType::BlurNode},
        // --
        {"Output node", NodeType::OutputNode},
    };

    bool Draw();

    NodeType GetNodeType() const;
    ImVec2 GetClickPos() const;

    // Node factory
    std::unique_ptr<NodeBase> CreateNode(NodeType type);
};
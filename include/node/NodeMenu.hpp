#pragma once

#include "imgui.h"
#include "imnodes.h"
#include <string>
#include <array>

#include "node/NodeBase.hpp"

// In 'n' out
#include "node/submodules/io/Input.hpp"
#include "node/submodules/io/Preview.hpp"
#include "node/submodules/io/Download.hpp"
// Common
#include "node/submodules/Monochrome.hpp"
#include "node/submodules/Brightness.hpp"
#include "node/submodules/Blur.hpp"
#include "node/submodules/RGB.hpp"

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
        RGBNode,
        PreviewNode,
        DownloadNode,
    } nodeType;

    struct MenuItem {
        const char* label;
        NodeType type;
    };

    // Always put the new nodes in between the input and download
    std::array<MenuItem, 7> items = {
        "Input node", NodeType::InputNode,
        // --
        "Monochrome", NodeType::MonochromeNode,
        "Brightness", NodeType::BrightnessNode,
        "Blur", NodeType::BlurNode,
        "RGB", NodeType::RGBNode,
        "Preview", NodeType::PreviewNode,
        // --
        "Download node", NodeType::DownloadNode,
    };

    bool Draw();

    NodeType GetNodeType() const;
    ImVec2 GetClickPos() const;

    // Node factory
    std::unique_ptr<NodeBase> CreateNode(NodeType type);
};

#pragma once

#include <imgui.h>
#include "imnodes.h"
#include "style.hpp"

#include <GLES3/gl3.h>

#include <memory>
#include <vector>
#include <chrono>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#include "ImageData.hpp"

enum class PinType { Input, Output, Both };

class Link {
public:
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

    std::chrono::steady_clock::time_point last_process_time;
    float process_delay_ms = 100.0f; // Default 100ms delay between updates
    bool needs_reprocess = false;

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

    virtual void ProcessInternal() = 0;
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

    virtual void Process() { // where nodes modify incoming data
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_process_time).count();

        if (elapsed >= process_delay_ms || needs_reprocess) {
            ProcessInternal();
            last_process_time = now;
            needs_reprocess = false;
        }
    }
    void Draw();

    void MarkNeedsReprocess() { needs_reprocess = true; }
    void SetProcessDelay(float delay_ms) { process_delay_ms = delay_ms; }

    ImageData input_image;
    ImageData output_image;

    bool IsProtected() const;
    int GetId() const;
    bool IsSelected() const;
    std::string GetInternalTitle() const;
    
    virtual ~NodeBase() = default;

    virtual void Description() = 0; // description for every node
    bool IsDeletable() const { return deletable; }

    int GetInputId() const { return n_input_id; }
    int GetOutputId() const { return n_output_id; }

    ImageData& GetInputImage() { return input_image; }
    ImageData& GetOutputImage() { return output_image; }
};

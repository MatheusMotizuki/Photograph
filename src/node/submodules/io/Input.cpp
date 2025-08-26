#include "node/submodules/io/Input.hpp"
#include <imgui.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

InputNode::InputNode() : NodeBase("Input Node", PinType::Output, false, ImVec4(0.2f, 0.7f, 1.0f, 1.0f)) {
    std::cout << "creating input node" << std::endl;
}

bool InputNode::ShouldDisplayText() const { return false; }

void InputNode::NodeContent() {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
    
    ImGui::BeginChild("image input", ImVec2(256, 128));
    cv::Mat image = cv::imread("assets/images/joia.jpg");
    ImGui::EndChild();

    ImGui::PopStyleVar(3);
    
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(18, 18, 18, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(61, 61, 61, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(31, 31, 31, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 160, 200, 255));

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[11]);

    if (ImGui::Button("Upload image", ImVec2(256, 30))) openDialog();

    ImGui::PopFont();

    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);
}

void InputNode::openDialog() {
    
}
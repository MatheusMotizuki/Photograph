#include "node/submodules/Monochrome.hpp"
#include <opencv2/imgproc.hpp>

MonochromeNode::MonochromeNode() : NodeBase("Monochrome Node", PinType::Both, "monochrome_node", true, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)) {}
MonochromeNode::~MonochromeNode() {}

unsigned int MonochromeNode::GetBorderColor() const {
    return IM_COL32(0, 255, 255, 255);
}

void MonochromeNode::NodeContent() {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
    // --
    ImGui::PopStyleVar(3);
}

void MonochromeNode::ProcessInternal() {
    if (!input_image.isValid()) return;
    
    output_image = input_image;
    
    // Create OpenCV Mat from image data
    cv::Mat img(output_image.height, output_image.width, 
                output_image.channels == 4 ? CV_8UC4 : CV_8UC3, 
                output_image.pixels.data());
    
    // Convert to grayscale using OpenCV (highly optimized)
    if (output_image.channels == 4) {
        cv::Mat gray;
        cv::cvtColor(img, gray, cv::COLOR_BGRA2GRAY);
        cv::cvtColor(gray, img, cv::COLOR_GRAY2BGRA);
    } else {
        cv::Mat gray;
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
        cv::cvtColor(gray, img, cv::COLOR_GRAY2BGR);
    }
}

void MonochromeNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.0f, 0.7f, 0.7f, 1.0f), "Monochrome Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Converts the input image to a monochrome (grayscale) version.");
        ImGui::Text("This node processes each pixel by calculating its luminance and ");
        ImGui::Text("setting the red, green, and blue channels to the same value, ");
        ImGui::Text("resulting in a black-and-white output.");
        ImGui::Spacing();
        ImGui::BulletText("Input: Image");
        ImGui::BulletText("Output: Image in monochrome scale");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Connect an image input to see the effect.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}
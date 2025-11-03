#include "node/submodules/Monochrome.hpp"

MonochromeNode::MonochromeNode() : NodeBase("Monochrome Node", PinType::Both, "monochrome_node", true, ImVec4(0.0f, 1.0f, 1.0f, 1.0f))
{
    std::cout << "Created monochrome node" << std::endl;
}
MonochromeNode::~MonochromeNode()
{
    std::cout << "destroyed monochrome node" << std::endl;
}

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
    int channels = output_image.channels;
    
    for (int i = 0; i < output_image.width * output_image.height; ++i) {
        unsigned char* pixel = &output_image.pixels[i * channels];
        
        // Calculate grayscale value using luminance formula
        unsigned char gray = static_cast<unsigned char>(
            0.299f * pixel[0] + 0.587f * pixel[1] + 0.114f * pixel[2]);
        
        // Set RGB channels to gray value
        pixel[0] = pixel[1] = pixel[2] = gray;
        // Keep alpha channel unchanged if it exists (pixel[3])
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
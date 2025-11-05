#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "node/submodules/io/Download.hpp"

DownloadNode::DownloadNode(SDL_Renderer* renderer) 
    : NodeBase("Download Node", PinType::Input, "download_node", false, ImVec4(0.3137f, 1.0f, 0.7059f, 1.0f))
    , m_renderer(renderer) {
    ImNodes::SetNodeScreenSpacePos(GetId(), ImVec2(900, 350));
}

DownloadNode::~DownloadNode() {}

bool DownloadNode::ShouldDisplayText() const { return false; }

void DownloadNode::NodeContent() {
    setStyle();

    ImGui::TextDisabled("Output Preview:");
    if (m_texture) {
        float m_width = 350.0f, m_height = 300.0f;
        float image_w = (float)m_tex_w, image_h = (float)m_tex_h;
        float scale = 1.0f;
        ImVec2 size;
        if (image_w > m_width) {
            scale = m_width / image_w;
            size = ImVec2(m_width, image_h * scale);
        } else if (image_h > m_height) {
            scale = m_height / image_h;
            size = ImVec2(image_w * scale, m_height);
        } else {
            size = ImVec2(image_w, image_h);
        }
        ImGui::BeginChild("##output_image", ImVec2(m_width, size.y), false);
        ImGui::Image((ImTextureID)(intptr_t)m_texture, size);
        ImGui::EndChild();
    } else {
        // do nothing!!11!
    }
    
    bool has_valid_input = input_image.isValid();
    if (!has_valid_input) {
        ImGui::BeginDisabled();
    }
    
    if (ImGui::Button("Download image", ImVec2(200, 30))) { 
        if (has_valid_input) {
            // Use stb_image_write to save the image as PNG
            std::string filename = "output_image.jpg";
            int quality = 90;
            std::cout << input_image.width * input_image.channels << std::endl;
            int success = stbi_write_jpg(
                filename.c_str(),
                input_image.width,
                input_image.height,
                input_image.channels,
                input_image.pixels.data(),
                quality
            );
            if (success) {
                std::cout << "Image downloaded to " << filename << std::endl;
            } else {
                std::cerr << "Failed to download image." << std::endl;
            }
        }
    }
    
    if (!has_valid_input) {
        ImGui::EndDisabled();
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("Connect an input to enable download");
        }
    }

    popStyle();
}

void DownloadNode::setStyle() {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(18, 18, 18, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(61, 61, 61, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(31, 31, 31, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 255, 180, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    // ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[15]);
}

void DownloadNode::popStyle() {
    // ImGui::PopFont();
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);
}

void DownloadNode::ClearPreview() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    m_tex_w = 0;
    m_tex_h = 0;
    output_image = ImageData();
}

void DownloadNode::ProcessInternal() {
    if (!input_image.isValid()) {
        ClearPreview(); // Clear if no valid input
        return;
    }

    // Only recreate texture if dimensions changed
    bool dimensions_changed = (!m_texture || 
        m_tex_w != input_image.width || 
        m_tex_h != input_image.height);
    
    if (dimensions_changed) {
        if (m_texture) {
            SDL_DestroyTexture(m_texture);
            m_texture = nullptr;
        }
        
        m_tex_w = input_image.width;
        m_tex_h = input_image.height;
        
        Uint32 format = (input_image.channels == 4) ? SDL_PIXELFORMAT_RGBA32 : SDL_PIXELFORMAT_RGB24;
        
        m_texture = SDL_CreateTexture(
            m_renderer,
            format,
            SDL_TEXTUREACCESS_STREAMING,
            m_tex_w,
            m_tex_h
        );
    }

    // Always update texture data when there's valid input
    if (m_texture) {
        SDL_UpdateTexture(
            m_texture,
            nullptr,
            input_image.pixels.data(),
            m_tex_w * input_image.channels
        );
    }
}

void DownloadNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.31f, 1.0f, 0.71f, 1.0f), "Download Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Allows you to download the final output image.");
        ImGui::Text("Use this node to save the processed image to your device.");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Connect your last processing node");
        ImGui::TextDisabled("to this node, then click 'Download image'.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}
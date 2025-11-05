#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "node/submodules/io/Download.hpp"

DownloadNode::DownloadNode() 
    : NodeBase("Download Node", PinType::Input, "download_node", false, ImVec4(0.3137f, 1.0f, 0.7059f, 1.0f)) {
    ImNodes::SetNodeScreenSpacePos(GetId(), ImVec2(900, 350));
}

DownloadNode::~DownloadNode() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
}

bool DownloadNode::CreateTextureFromData(unsigned char* data, int width, int height, int channels) {
    if (!data) return false;

    // Delete old texture if exists
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }

    // Generate OpenGL texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Determine format based on channels
    GLenum format = GL_RGBA;
    if (channels == 1) format = GL_RED;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_tex_w = width;
    m_tex_h = height;

    return true;
}

bool DownloadNode::ShouldDisplayText() const { return false; }

void DownloadNode::NodeContent() {
    setStyle();

    ImGui::TextDisabled("Output Preview:");
    
    // Display texture if loaded (same as InputNode)
    if (m_texture && m_tex_w > 0 && m_tex_h > 0) {
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
        // Cast GLuint to ImTextureID (which is void* in OpenGL backend)
        ImGui::Image((ImTextureID)(intptr_t)m_texture, size);
        ImGui::EndChild();
    }
    
    bool has_valid_input = input_image.isValid();
    if (!has_valid_input) {
        ImGui::BeginDisabled();
    }
    
    if (ImGui::Button("Download image", ImVec2(200, 30))) { 
        if (has_valid_input) {
            // Browser-based download using JavaScript
            // Encode the image to PNG in memory
            int png_size = 0;
            unsigned char* png_data = stbi_write_png_to_mem(
                input_image.pixels.data(),
                input_image.width * input_image.channels,
                input_image.width,
                input_image.height,
                input_image.channels,
                &png_size
            );
            
            if (png_data && png_size > 0) {
                // Use EM_ASM to trigger download in browser
                EM_ASM({
                    var pngData = new Uint8Array(HEAPU8.buffer, $0, $1);
                    var blob = new Blob([pngData], { type: 'image/png' });
                    var url = URL.createObjectURL(blob);
                    var a = document.createElement('a');
                    a.href = url;
                    a.download = 'processed_image.png';
                    document.body.appendChild(a);
                    a.click();
                    document.body.removeChild(a);
                    URL.revokeObjectURL(url);
                }, png_data, png_size);
                
                STBIW_FREE(png_data);
                std::cout << "Image download started (browser)" << std::endl;
            } else {
                std::cerr << "Failed to encode image to PNG." << std::endl;
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
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 255, 180, 255));
    
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
}

void DownloadNode::popStyle() {
    ImGui::PopFont();
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);
}

void DownloadNode::ClearPreview() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
    m_tex_w = 0;
    m_tex_h = 0;
    m_last_pixels.clear();
    output_image = ImageData();
}

void DownloadNode::ProcessInternal() {
    std::cout << "[DownloadNode] ProcessInternal called. Input valid: " << input_image.isValid() << std::endl;
    if (!input_image.isValid()) {
        ClearPreview();
        return;
    }

    // Check if texture doesn't exist or if pixel data has actually changed
    bool needs_update = !m_texture || m_last_pixels != input_image.pixels;
    
    if (needs_update) {
        std::cout << "[DownloadNode] Updating texture with new image data." << std::endl;
        CreateTextureFromData(
            input_image.pixels.data(),
            input_image.width,
            input_image.height,
            input_image.channels
        );
        // Cache the current pixel data
        m_last_pixels = input_image.pixels;
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

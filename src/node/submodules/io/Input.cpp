#include "node/submodules/io/Input.hpp"

InputNode::InputNode() 
    : NodeBase("Input Node", PinType::Output, "input_node", false, ImVec4(0.2f, 0.7f, 1.0f, 1.0f))
{
    ImNodes::SetNodeScreenSpacePos(GetId(), ImVec2(60, 50));
}

InputNode::~InputNode() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
    if (m_image_data) {
        stbi_image_free(m_image_data);
        m_image_data = nullptr;
    }
}

bool InputNode::CreateTextureFromData(unsigned char* data, int width, int height, int channels) {
    if (!data) return false;

    // Delete old texture if exists
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
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

bool InputNode::ShouldDisplayText() const { return false; }

void InputNode::NodeContent() {
    ImGui::TextDisabled("Input Preview:");
    setStyle();

    // Display texture if loaded
    if (m_texture && m_image_data) {
        float m_width = 200.f, m_height = 200.f; // cap the image size
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

        ImGui::BeginChild("##input image", ImVec2(m_width, size.y), false);
        // Cast GLuint to ImTextureID (which is void* in OpenGL backend)
        ImGui::Image((ImTextureID)(intptr_t)m_texture, size);
        ImGui::EndChild();
    }

    // Button to load image
    if (ImGui::Button("Load Image", ImVec2(-1, 0))) {
        // TODO: Implement file loading
        // For web: you'll need to use HTML5 file API
        // For native: use your FilePicker
        
#ifndef __EMSCRIPTEN__
        // Native file loading example
        // std::string filepath = filePicker.OpenFile("Image Files", "*.png;*.jpg;*.jpeg;*.bmp");
        std::string filepath = "";
        if (!filepath.empty()) {
            int width, height, channels;
            unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 4); // Force RGBA
            if (data) {
                if (m_image_data) {
                    stbi_image_free(m_image_data);
                }
                m_image_data = data;
                CreateTextureFromData(data, width, height, 4);
            }
        }
#else
        // For web, you'll need to implement HTML5 file input
        ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Web file loading not yet implemented");
#endif
    }

    popStyle();
}

void InputNode::setStyle() {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(18, 18, 18, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(61, 61, 61, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(31, 31, 31, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 160, 200, 255));

#ifdef __EMSCRIPTEN__
    // Use default font on web
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
#else
    // Check if font exists before accessing
    if (ImGui::GetIO().Fonts->Fonts.Size > 15) {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[15]);
    } else {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    }
#endif
}

void InputNode::popStyle() {
    ImGui::PopFont();
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);
}

void InputNode::Process() {
    
}

void InputNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.2f, 0.7f, 1.0f, 1.0f), "Input Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Loads an image from your computer");
        ImGui::Text("and provides it as the starting point");
        ImGui::Text("for your node graph.");
        ImGui::Spacing();
        ImGui::Text("Supported formats include PNG, JPG,");
        ImGui::Text("and other common image types.");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Use this node to import");
        ImGui::TextDisabled("photos or textures for editing.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}

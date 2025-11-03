#include "node/submodules/io/Preview.hpp"

PreviewNode::PreviewNode() 
    : NodeBase("Preview Node", PinType::Both, "preview_node", true, ImVec4(0.4f, 0.6f, 1.0f, 1.0f)) {}

PreviewNode::~PreviewNode() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
}

bool PreviewNode::CreateTextureFromData(unsigned char* data, int width, int height, int channels) {
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

unsigned int PreviewNode::GetBorderColor() const {
    return IM_COL32(102, 153, 255, 255);
}

bool PreviewNode::ShouldDisplayText() const { return false; }

void PreviewNode::NodeContent() {
    ImGui::TextDisabled("Preview:");
    setStyle();

    // Display texture if loaded (same as InputNode and DownloadNode)
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
        
        ImGui::BeginChild("##preview_image", ImVec2(m_width, size.y), false);
        // Cast GLuint to ImTextureID (which is void* in OpenGL backend)
        ImGui::Image((ImTextureID)(intptr_t)m_texture, size);
        ImGui::EndChild();
    } else {
        // // Show placeholder when no image is connected
        // ImGui::BeginChild("##preview_placeholder", ImVec2(350.0f, 200.0f), true);
        // ImVec2 text_size = ImGui::CalcTextSize("No input connected");
        // ImVec2 window_size = ImGui::GetWindowSize();
        // ImGui::SetCursorPos(ImVec2(
        //     (window_size.x - text_size.x) * 0.5f,
        //     (window_size.y - text_size.y) * 0.5f
        // ));
        // ImGui::TextDisabled("No input connected");
        // ImGui::EndChild();
    }

    popStyle();
}

void PreviewNode::setStyle() {
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 255, 180, 255));
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

void PreviewNode::popStyle() {
    ImGui::PopFont();
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(2);
}

void PreviewNode::ClearPreview() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
    m_tex_w = 0;
    m_tex_h = 0;
    output_image = ImageData();
}

void PreviewNode::ProcessInternal() {
    if (!input_image.isValid()) {
        ClearPreview();
        return;
    }

    // Only recreate texture if dimensions changed or texture doesn't exist
    bool needs_update = (!m_texture || 
        m_tex_w != input_image.width || 
        m_tex_h != input_image.height);
    
    if (needs_update) {
        CreateTextureFromData(
            input_image.pixels.data(),
            input_image.width,
            input_image.height,
            input_image.channels
        );
    }
    
    // Pass through the input to output (so other nodes can chain from preview)
    output_image = input_image;
}

void PreviewNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "Preview Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Displays a preview of your node graph output.");
        ImGui::Text("Use this node to see intermediate results");
        ImGui::Text("or the final processed image.");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: You can chain multiple preview nodes");
        ImGui::TextDisabled("to inspect different stages of processing.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}